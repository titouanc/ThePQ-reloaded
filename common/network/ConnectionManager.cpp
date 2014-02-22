#include "ConnectionManager.hpp"
#include "Exception.hpp"

extern "C" {
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
}

using namespace net;

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
