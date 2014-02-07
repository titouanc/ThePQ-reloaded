#ifndef __SOCKET_HPP
#define __SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Socket {
  public:
    virtual ~ Socket();

    enum Status {
	OK,
	ERROR
    };

  protected:
     Socket();
    bool create();
    void close();
    bool isOpen() {
	return _sockfd >= 0;
    };

    int _sockfd;
    struct sockaddr_in _servAddr;
    struct sockaddr_in _cliAddr;
};

#endif				// __SOCKET_HPP
