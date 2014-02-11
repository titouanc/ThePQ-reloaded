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
    const char *bind_addr_repr, 
    unsigned short bind_port,
    int max_clients
) : _sockfd(-1), _clients()
{
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
    std::map<int, User>::iterator it;
    for (it=_clients.begin(); it!=_clients.end(); it++){
        close(it->first);
    }
}

void ConnectionManager::_removeClient(int client_id)
{
    std::cout << "REMOVE CLIENT: " << client_id << std::endl;
    std::map<int, User>::iterator pos = _clients.find(client_id);
    if (pos != _clients.end()){
        _clients.erase(pos);
        close(client_id);
    }
}

void ConnectionManager::_addClient(void)
{
    int fd = accept(_sockfd, NULL, NULL);
    if (fd < 0)
        throw ConnectionError(std::string("Accept error: ")+strerror(errno));
    std::cout << "NEW CLIENT: " << fd << std::endl;
    _clients.insert(_clients.begin(), std::pair<int, User>(fd, User()));
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

void ConnectionManager::mainloop(SharedQueue<Message> & incoming)
{
    fd_set readable, errors;
    std::map<int, User>::const_iterator it;
    int fdmax = _sockfd;

    FD_ZERO(&readable);
    FD_ZERO(&errors);
    FD_SET(_sockfd, &readable);
    for (it=_clients.begin(); it!=_clients.end(); it++){
        FD_SET(it->first, &readable);
        FD_SET(it->first, &errors);
        if (it->first > fdmax)
            fdmax = it->first;
    }

    if (select(fdmax+1, &readable, NULL, &errors, NULL) > 0){
        if (FD_ISSET(_sockfd, &readable))
            _addClient();
        for (it=_clients.begin(); it!=_clients.end(); it++){
            int client_id = it->first;
            if (FD_ISSET(client_id, &readable)){
                try {
                    JSON::Value *msg = _readFrom(client_id);
                    if (msg)
                        incoming.push(Message(client_id, msg));
                    else {
                        /* select() returned ready for read, but nothing has 
                           been read => close connection */
                        close(it->first);
                        _clients.erase(it);
                    }
                } catch (JSON::ParseError &err) {}
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
