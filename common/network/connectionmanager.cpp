#include "connectionmanager.h"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <json.h>

extern "C" {
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
}

ConnectionManager::ConnectionManager( 
    SharedQueue<Message> & incoming_queue,
    const char *bind_addr_repr, 
    unsigned short bind_port,
    int max_clients
) : _sockfd(-1), _clients(), _running(false), _incoming(incoming_queue)
{
    pthread_mutex_init(&_mutex, NULL);

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        throw ConnectionError(
            std::string("Unable to open socket: ")+
            strerror(errno)
        );

    memset(&_bind_addr, 0, sizeof(struct sockaddr_in));
    _bind_addr.sin_family = AF_INET;
    _bind_addr.sin_port = htons(bind_port);
    if (inet_aton(bind_addr_repr, &(_bind_addr.sin_addr)) == 0){
        close(_sockfd);
        throw ConnectionError(
            std::string("Unable to parse adress ")+
            bind_addr_repr+strerror(errno)
        );
    }

    if (bind(_sockfd, (struct sockaddr *) &_bind_addr, sizeof(struct sockaddr_in)) != 0){
        close(_sockfd);
        throw ConnectionError(
            std::string("Unable to bind to ")+
            bind_addr_repr+strerror(errno)
        );
    }

    if (listen(_sockfd, max_clients) != 0){
        close(_sockfd);
        throw ConnectionError(
            std::string("Unable to listen on ")+
            bind_addr_repr+strerror(errno)
        );
    }
}

ConnectionManager::~ConnectionManager()
{
    std::set<int>::iterator it;
    for (it=_clients.begin(); it!=_clients.end(); it++){
        close(*it);
    }
    pthread_mutex_destroy(&_mutex);
}

JSON::Value *ConnectionManager::_addClient(void)
{
    int fd = accept(_sockfd, NULL, NULL);
    if (fd < 0)
        throw ConnectionError(std::string("Accept error: ")+strerror(errno));
    std::cout << "NEW CLIENT: " << fd << std::endl;
    _clients.insert(_clients.begin(), fd);
    JSON::Dict *msg = new JSON::Dict();
    msg->set("__type__", "CONNECT");
    msg->set("client_id", fd);
    return msg;
}

#define BUFSIZE 0x1000
JSON::Value *ConnectionManager::_readFrom(int fd)
{
    std::stringstream res;
    char buffer[BUFSIZE+1];
    int r, i=0;
    while ((r = recv(fd, buffer, BUFSIZE, 0)) > 0){
        buffer[r] = '\0';
        res << buffer;
        i++;
    }
    if (r < 0 || i == 0)
        return NULL;

    return JSON::parse(res.str().c_str());
}

JSON::Value *ConnectionManager::_removeClient(std::set<int>::iterator position)
{
    int client_id = *position;
    close(client_id);
    _clients.erase(position);

    JSON::Dict *msg = new JSON::Dict();
    msg->set("__type__", "DISCONNECT");
    msg->set("client_id", client_id);
    return msg;
}

bool ConnectionManager::_writeTo(int fd, JSON::Value *obj)
{
    if (obj != NULL){
        std::string const & repr = obj->dumps();
        int r = 0;
        for (size_t i=0; i<repr.length(); i+=r){
            r = send(fd, repr.c_str()+i, repr.length()-i, 0);
            if (r < 0)
                return false;
        }
    }
    return true;
}

void ConnectionManager::mainloop(void)
{
    fd_set readable;
    std::set<int>::iterator it;
    int fdmax;

    while (isRunning()){
        fdmax = _sockfd;
        FD_ZERO(&readable);
        FD_SET(_sockfd, &readable);

        for (it=_clients.begin(); it!=_clients.end(); it++){
            FD_SET(*it, &readable);
            if (*it > fdmax)
                fdmax = *it;
        }

        if (select(fdmax+1, &readable, NULL, NULL, NULL) > 0){
            if (FD_ISSET(_sockfd, &readable)){
                JSON::Value *msg = _addClient();
                if (msg)
                    _incoming.push(Message(*it, msg));
            }
            for (it=_clients.begin(); it!=_clients.end();){
                if (! FD_ISSET(*it, &readable))
                    it++; /* fd not set; examine next */
                else {
                    try {
                        JSON::Value *msg = _readFrom(*it);
                        if (msg){
                            _incoming.push(Message(*it, msg));
                            it++;
                        } else {
                            /* select() returned ready for read, but nothing has 
                               been read => close connection */
                            std::set<int>::iterator to_remove = it;
                            it++;
                            msg = _removeClient(to_remove);
                            if (msg)
                                _incoming.push(Message(_sockfd, msg));
                        }
                    } catch (JSON::ParseError &err) {}
                }
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

typedef void*(*pthread_routine_t)(void*);
static void runConnectionManager(void *args)
{
    ConnectionManager *manager = (ConnectionManager*) args;
    manager->mainloop();
    pthread_exit(NULL);
}

bool ConnectionManager::start(void)
{
    bool res = false;
    int lock = pthread_mutex_lock(&_mutex);
    if (lock == 0){
        if (! _running){
            _running = true;
            int r = pthread_create(
                &_io_thread, NULL, 
                (pthread_routine_t) runConnectionManager, this
            );
            res = (r == 0);
        }
        pthread_mutex_unlock(&_mutex);
    }
    return res;
}

bool ConnectionManager::stop(void)
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

bool ConnectionManager::isRunning(void)
{
    bool res = false;
    int lock = pthread_mutex_lock(&_mutex);
    if (lock == 0){
        res = _running;
        pthread_mutex_unlock(&_mutex);
    }
    return res;
}
