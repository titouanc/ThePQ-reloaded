#include "net.hpp"

bool net::Socket::create()
{
    /* Initialization */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return isOpen();	// Socket created
}

void net::Socket::close()
{
    if (isOpen())
    {
		::close(_sockfd);
	}
}

net::Socket::Socket()
{
	_sockfd = -1;
}

net::Socket::~Socket()
{
    close();
}
