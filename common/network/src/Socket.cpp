#include "net.hpp"

bool net::Socket::create()
{
    /* Initialization */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return _sockfd >= 0;	// Socket created
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
	
}

net::Socket::~Socket()
{
    close();
}
