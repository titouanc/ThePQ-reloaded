#include "ConnectionManager.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <cstdint>

extern "C" {
    #include <netdb.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
}

using namespace net;

/* ===== BaseConnectionManager ===== */
BaseConnectionManager::BaseConnectionManager( 
    SharedQueue<Message> & incoming_queue,
    SharedQueue<Message> & outgoing_queue,
    bool logger
) : _incoming(incoming_queue), _outgoing(outgoing_queue), _logger(logger)
{
    pthread_mutex_init(&_mutex, NULL);
    pthread_mutex_init(&_fdset_mutex, NULL);
}

BaseConnectionManager::~BaseConnectionManager()
{
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

bool BaseConnectionManager::_doWrite(int fd, JSON::Value *obj)
{
    if (obj != NULL){
        std::string const & repr = obj->dumps();
        uint32_t msglen = htonl(repr.length());
        int r = send(fd, &msglen, 4, 0);
        if (r != 4)
            return false;

        for (size_t i=0; i<repr.length(); i+=r){
            r = send(fd, repr.c_str()+i, repr.length()-i, 0);
            if (r < 0)
                return false;
        }
        if (_logger)
            std::cout << "\033[1m" << fd << " \033[36m<<\033[0m " 
                      << *obj << std::endl;
    }
    return true;
}

#define BUFSIZE 0x1000
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

    while (msglen > 0 && (r = recv(fd, buffer, BUFSIZE, 0)) > 0){
        buffer[r] = '\0';
        globalBuf << buffer;
        i++;
        msglen -= r;
    }
    if (r < 0 || i == 0)
        return false;

    JSON::Value *res = JSON::parse(globalBuf.str().c_str());
    if (res != NULL){
        _incoming.push(Message(fd, res));
        if (_logger)
            std::cout << "\033[1m" << fd << " \033[33m>>\033[0m " 
                      << *res << std::endl;
    }

    return true;
}

void BaseConnectionManager::_doDisconnect(int fd)
{
    close(fd);
    removeClient(fd);
    JSON::Dict msgdata;
    msgdata.set("type", "DISCONNECT");
    msgdata.set("client_id", fd);
    _incoming.push(Message(fd, msgdata.clone()));
    if (_logger)
		std::cout << "\033[1m" << fd << " \033[35m disconnected\033[0m" << std::endl;
}

void BaseConnectionManager::_doConnect(int fd)
{
    addClient(fd);
    JSON::Dict msgdata;
    msgdata.set("type", "CONNECT");
    msgdata.set("client_id", fd);
    _incoming.push(Message(fd, msgdata.clone()));
    std::cout << "\033[1m" << fd << " \033[34m connected\033[0m" << std::endl;
}

int BaseConnectionManager::_doSelect(int fdmax, fd_set *readable)
{
    pthread_mutex_lock(&_fdset_mutex);
    std::set<int>::iterator it;
    for (it=_clients.begin(); it!=_clients.end(); it++){
        FD_SET(*it, readable);
        if (*it > fdmax)
            fdmax = *it;
    }
    pthread_mutex_unlock(&_fdset_mutex);

    int res = select(fdmax+1, readable, NULL, NULL, NULL);

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
        pthread_mutex_lock(&_fdset_mutex);
        std::set<int>::iterator pos = _clients.find(to_send.peer_id);
        if (pos != _clients.end())
            _doWrite(to_send.peer_id, to_send.data);
        pthread_mutex_unlock(&_fdset_mutex);
        delete to_send.data;
    }
}

void BaseConnectionManager::addClient(int fd)
{
    pthread_mutex_lock(&_fdset_mutex);
    _clients.insert(fd);
    pthread_mutex_unlock(&_fdset_mutex);
}

bool BaseConnectionManager::removeClient(int fd)
{
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

/* ===== ConnectionManager ===== */
ConnectionManager::ConnectionManager( 
    SharedQueue<Message> & incoming_queue,
    SharedQueue<Message> & outgoing_queue,
    const char *bind_addr_repr, 
    unsigned short bind_port,
    int max_clients
) : BaseConnectionManager::BaseConnectionManager(incoming_queue, outgoing_queue, true),
   _sockfd(-1)
{
    int yes = 1;

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        throw ConnectionFailedException();

    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
        throw ConnectionFailedException();
    }

    memset(&_bind_addr, 0, sizeof(struct sockaddr_in));
    _bind_addr.sin_family = AF_INET;
    _bind_addr.sin_port = htons(bind_port);
    if (inet_aton(bind_addr_repr, &(_bind_addr.sin_addr)) == 0){
        close(_sockfd);
        throw ConnectionFailedException();
    }

    if (bind(_sockfd, (struct sockaddr *) &_bind_addr, sizeof(struct sockaddr_in)) != 0){
        close(_sockfd);
        throw ConnectionFailedException();
    }

    if (listen(_sockfd, max_clients) != 0){
        close(_sockfd);
        throw ConnectionFailedException();
    }
}

ConnectionManager::~ConnectionManager()
{
    close(_sockfd);
}

void ConnectionManager::_mainloop_in(void)
{
    fd_set readable;

    while (isRunning()){
        FD_ZERO(&readable);
        FD_SET(_sockfd, &readable);
        _doSelect(_sockfd, &readable);
        if (FD_ISSET(_sockfd, &readable)){
            int fd = accept(_sockfd, NULL, NULL);
            if (fd > 0){
                _doConnect(fd);
            }
        }
    }
}

const char *ConnectionManager::ip(void) const
{
    return inet_ntoa(_bind_addr.sin_addr);
}

unsigned short ConnectionManager::port(void) const
{
    return ntohs(_bind_addr.sin_port);
}

/* ===== SubConnectionManager ===== */
SubConnectionManager::SubConnectionManager(
    SharedQueue<Message> & incoming_queue,
    SharedQueue<Message> & outgoing_queue,
    BaseConnectionManager & parent
) : 
BaseConnectionManager(incoming_queue, outgoing_queue), _parent(parent)
{}

bool SubConnectionManager::acquireClient(int client_id)
{
    if (_parent.removeClient(client_id)){
        addClient(client_id);
        return true;
    }
    return false;
}

bool SubConnectionManager::releaseClient(int client_id)
{
    if (removeClient(client_id)){
        _parent.addClient(client_id);
        return true;
    }
    return false;
}

SubConnectionManager::~SubConnectionManager()
{
    /* Give back fd to parent connection on destruction */
    for (iterator client=_iterClients(); client!=_iterEnd();){
        iterator next = client;
        next++;
        releaseClient(*client);
        client = next;
    }
}

ClientConnectionManager::ClientConnectionManager(
				SharedQueue<Message> & incoming_queue,
				SharedQueue<Message> & outgoing_queue,
				const char *host_addr, 
				unsigned short host_port
) : BaseConnectionManager::BaseConnectionManager(incoming_queue, outgoing_queue, false),
	_sockfd(-1)
{
    struct hostent *host = gethostbyname(host_addr);
    if (host == NULL){
        throw ConnectionFailedException(
            std::string("Could not resolve host ")+host_addr
        );
    }

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        throw ConnectionFailedException();

    memset(&_host_addr, 0, sizeof(struct sockaddr_in));
    
    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = htons(host_port);
    _host_addr.sin_addr = *((struct in_addr*) *(host->h_addr_list));

    if (connect(_sockfd, (struct sockaddr *) &_host_addr,
		sizeof(_host_addr)) < 0)
	{
		close(_sockfd);
		throw ConnectionFailedException();
	}

    addClient(_sockfd);
}

void ClientConnectionManager::_mainloop_in()
{
	while(isRunning())
	{
		if(_doRead(_sockfd) == false)
		{
			_doDisconnect(_sockfd);
			stop();
		}
	}
}

void ClientConnectionManager::_mainloop_out()
{
    while (isRunning()){
        Message const & msg = _outgoing.pop();
        _doWrite(_sockfd, msg.data);
        delete msg.data;
    }
}

const char *ClientConnectionManager::ip(void) const
{
    return inet_ntoa(_host_addr.sin_addr);
}

unsigned short ClientConnectionManager::port(void) const
{
    return ntohs(_host_addr.sin_port);
}
