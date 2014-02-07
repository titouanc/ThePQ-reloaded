#include "Socket.hpp"

bool Socket::create()
{
    /* Initialization */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return _sockfd >= 0;	// Socket created
}

void Socket::close()
{
    if (isOpen())
    {
		::close(_sockfd);
	}
}

Socket::~Socket()
{
    close();
}
