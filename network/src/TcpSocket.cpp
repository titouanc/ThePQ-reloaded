#include "TcpSocket.hpp"
#include <arpa/inet.h>

Socket::Status TcpSocket::connect(const std::string ipAddr, int portNo)
{
    if (! create())
		return Status::ERROR;
    
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    _servAddr.sin_port = htons(portNo);
    
    if (::connect(_sockfd,(struct sockaddr *)&_servAddr,sizeof(_servAddr)) < 0) 
        return Status::ERROR;
    return Status::OK;  	
}

Socket::Status TcpSocket::send(const void *data, size_t len)
{
	
}

Socket::Status TcpSocket::recv(void *data, size_t len, size_t &received)
{
	
}
