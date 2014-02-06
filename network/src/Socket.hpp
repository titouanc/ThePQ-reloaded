#ifndef __SOCKET_HPP
#define __SOCKET_HPP

//~ #include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//~ #include <netdb.h>
#include <unistd.h>

class Socket
{
public:
	virtual ~Socket();
	
	enum Status
	{
		OK,
		ERROR
	};
	
protected:
	Socket();
	virtual Status create();
	void close();
	
	int _sockfd;
    struct sockaddr_in _servAddr;
    struct sockaddr_in _cliAddr;	
};

#endif // __SOCKET_HPP
