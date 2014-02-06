#include "Socket.hpp"

Socket::Status Socket::create()
{
	/* Initialization */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        return Status::ERROR;	
}

void Socket::close()
{
	::close(_sockfd);	
}

Socket::~Socket()
{
	close();
}
