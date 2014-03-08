#include "BaseConnectionManager.hpp"
#include <Constants.hpp>

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <cstdint>

extern "C" {
    #include <errno.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
}

using namespace net;

/* ===== BaseConnectionManager ===== */
BaseConnectionManager::BaseConnectionManager( 
    /*Constructor*/
    SharedQueue<Message> & incoming_queue,
    SharedQueue<Message> & outgoing_queue,
    bool logger
) : 
    _running(false), 
    _tx_bytes(0), _rx_bytes(0),
    _incoming(incoming_queue), _outgoing(outgoing_queue),
    _logger(logger)
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_mutex_init(&_fdset_mutex, NULL);
}

BaseConnectionManager::~BaseConnectionManager()
{
    /*Destructor*/
    stop();
    std::set<int>::iterator it, next;
    for (it=_clients.begin(); it!=_clients.end();) {
		next = it;
		next++;
        _doDisconnect(*it);
		it = next;
    }
    pthread_mutex_destroy(&_mutex);
    pthread_mutex_destroy(&_fdset_mutex);
}

bool BaseConnectionManager::_doWrite(int fd, const JSON::Value *obj)
{
    bool res = true;
    if (obj != NULL){
        pthread_mutex_lock(&_fdset_mutex);
        if (_clients.find(fd) != _clients.end()){
            std::string const & repr = obj->dumps();
            uint32_t msglen = htonl(repr.length());
            int r = send(fd, &msglen, 4, 0);
            if (r != 4)
                res = false;
            else {
                _tx_bytes += 4;

                for (size_t i=0; i<repr.length(); i+=r){
                    r = send(fd, repr.c_str()+i, repr.length()-i, 0);
                    if (r < 0){
                        res = false;
                        break;
                    }
                    _tx_bytes += r;
                }
                if (_logger && res)
                    std::cout << "[" << this << "] " << "\033[1m" << fd 
                              << " \033[36m<<\033[0m " << *obj << std::endl;
            }
        }
        pthread_mutex_unlock(&_fdset_mutex);
    }
    return res;
}

#define BUFSIZE 0x1000
#define min(a, b) ((a) < (b)) ? (a) : (b)
bool BaseConnectionManager::_doRead(int fd)
{
    std::stringstream globalBuf;
    char buffer[BUFSIZE+1];
    int r, i=0;

    uint32_t msglen;
    r = recv(fd, &msglen, 4, 0);
    if (r != 4)
        return false;
    msglen = ntohl(msglen);
    _rx_bytes += 4;

    while (msglen > 0 && (r = recv(fd, buffer, min(msglen, BUFSIZE), 0)) > 0){
        _rx_bytes += r;
        buffer[r] = '\0';
        globalBuf << buffer;
        i++;
        msglen -= r;
    }
    if (r < 0 || i == 0)
        return false;

    JSON::Value *res = JSON::parse(globalBuf.str().c_str());
    if (res != NULL){
        if (ISDICT(res) && 
            ISSTR(DICT(res).get("type")) &&
            DICT(res).hasKey("data")
        ){
            if (_logger)
                std::cout << "[" << this << "] "<< "\033[1m" << fd 
                          << " \033[33m>>\033[0m " << *res << std::endl;

            _incoming.push(Message(fd, res));
        } else {
            delete res;
        }
    }

    return true;
}

void BaseConnectionManager::_doDisconnect(int fd)
{
    /*Method performing the disconnect between the server and the client*/
    close(fd);
    removeClient(fd);
    JSON::Dict msgdata;
    msgdata.set("type", MSG::DISCONNECT);
    msgdata.set("client_id", fd);
    _incoming.push(Message(fd, msgdata.clone()));
    if (_logger)
		std::cout << "[" << this << "] "<< "\033[1m" << fd 
                  << " \033[35m disconnected\033[0m" << std::endl;
}

void BaseConnectionManager::_doConnect(int fd)
{
    /*Method performing the connection between the server and the client*/
    addClient(fd);
    JSON::Dict msgdata;
    msgdata.set("type", "CONNECT");
    msgdata.set("client_id", fd);
    _incoming.push(Message(fd, msgdata.clone()));
    std::cout << "[" << this << "] "<< "\033[1m" << fd 
              << " \033[34m connected\033[0m" << std::endl;
}

int BaseConnectionManager::_doSelect(int fdmax, fd_set *readable)
{
    /*Method allowing the selection of the connection
     * of the client
     */
    pthread_mutex_lock(&_fdset_mutex);
    std::set<int>::iterator it;
    for (it=_clients.begin(); it!=_clients.end(); it++){
        FD_SET(*it, readable);
        if (*it > fdmax)
            fdmax = *it;
    }
    pthread_mutex_unlock(&_fdset_mutex);

    timeval timeout = {0, 100000};
    int res = select(fdmax+1, readable, NULL, NULL, &timeout);

    if (res > 0){
        pthread_mutex_lock(&_fdset_mutex);
        for (it=_clients.begin(); it!=_clients.end(); ){
            int fd = *it;
            it++; /* Increment iterator before eventually removing client */
            if (FD_ISSET(fd, readable) && ! _doRead(fd)){
                /* FD ready but read error: close connection */
                pthread_mutex_unlock(&_fdset_mutex);
                _doDisconnect(fd);
                pthread_mutex_lock(&_fdset_mutex);
            }
        }
        pthread_mutex_unlock(&_fdset_mutex);
    }

    return res;
}

void BaseConnectionManager::_mainloop_in(void)
{
    fd_set readable;

    while (isRunning()){
        FD_ZERO(&readable);
        _doSelect(0, &readable);
    }
}

void BaseConnectionManager::_mainloop_out(void)
{
    while (isRunning()){
        Message const & to_send = _outgoing.pop();
        _doWrite(to_send.peer_id, to_send.data);
        delete to_send.data;
    }
}

size_t BaseConnectionManager::nClients(void)
{
    return _clients.size();
}

void BaseConnectionManager::addClient(int fd)
{
    /*Method allowing the addition of a client to the server*/
    pthread_mutex_lock(&_fdset_mutex);
    _clients.insert(fd);
    pthread_mutex_unlock(&_fdset_mutex);
}

bool BaseConnectionManager::removeClient(int fd)
{
    /*Method removing a client from the server
     *returns a boolean if disconnect successful
     *false otherwise
     */
    bool res = false;
    pthread_mutex_lock(&_fdset_mutex);
    if (_clients.find(fd) != _clients.end()){
        _clients.erase(fd);
        res = true;
    }
    pthread_mutex_unlock(&_fdset_mutex);
    return res;
}

/* PThread routines */
typedef void*(*pthread_routine_t)(void*);
static void runConnectionManagerIn(void *args)
{
    BaseConnectionManager *manager = (BaseConnectionManager*) args;
    manager->_mainloop_in();
    pthread_exit(NULL);
}

static void runConnectionManagerOut(void *args)
{
    BaseConnectionManager *manager = (BaseConnectionManager*) args;
    manager->_mainloop_out();
    pthread_exit(NULL);
}

bool BaseConnectionManager::start(void)
{
    bool res = false;
    int lock = pthread_mutex_lock(&_mutex);
    if (lock == 0){
        if (! _running){
            _running = true;
            int r = pthread_create(
                &_in_thread, NULL, 
                (pthread_routine_t) runConnectionManagerIn, this
            );
            if (r == 0)
                r = pthread_create(
                    &_out_thread, NULL,
                    (pthread_routine_t) runConnectionManagerOut, this
                );
            res = (r == 0);
        }
        pthread_mutex_unlock(&_mutex);
    }
    return res;
}

bool BaseConnectionManager::stop(void)
{
    bool res = false;
    int lock = pthread_mutex_lock(&_mutex);
    if (lock == 0){
        _running = false;
        pthread_mutex_unlock(&_mutex);
        res = true;
    }
    return res;
}

bool BaseConnectionManager::isRunning(void)
{
    bool res = false;
    int lock = pthread_mutex_lock(&_mutex);
    if (lock == 0){
        res = _running;
        pthread_mutex_unlock(&_mutex);
    }
    return res;
}
