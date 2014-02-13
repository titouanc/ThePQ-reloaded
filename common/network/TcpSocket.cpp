#include "net.hpp"
#include <cstring>

void net::TcpSocket::connect(const std::string ipAddr, int portNo)
{
    close();
    if (!create())
    {
		throw ConnectionFailedException();
	}

    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    _servAddr.sin_port = htons(portNo);

    if (::connect(_sockfd, (struct sockaddr *) &_servAddr,
		sizeof(_servAddr)) < 0)
	{
		throw ConnectionFailedException();
	}
}

void net::TcpSocket::send(const char *data, size_t len)
{
    if (::send(_sockfd, data, len, 0) <= 0)
    {
		throw DisconnectedException();
	}
}

void net::TcpSocket::recv(char *data, size_t len, size_t & received)
{
	data = new char[MSG_SIZE];
	memset(data, 0, MSG_SIZE);
    received = ::recv(_sockfd, data, len, 0);
    if (received <= 0)
    {
		throw DisconnectedException();
	}
	data[received] = '\0';
}

void net::TcpSocket::send(const JSON::Value *json)
{
	std::string dump = json->dumps();
	const char* data = (dump.c_str());
	uint32_t len = htonl(dump.length());
	int r = ::send (_sockfd, &len, 4, 0);
	if (r != 4)
		throw DisconnectedException();
    if (::send(_sockfd, data, dump.length(), 0) <= 0)
    {
		throw DisconnectedException();
	}
}

JSON::Value* net::TcpSocket::recv()
{
	JSON::Value* json;
	char* data = new char[MSG_SIZE];
	size_t received;
	bzero(data, MSG_SIZE);
	uint32_t len = 0;
	int r = ::recv(_sockfd, &len, 4, 0);
	len = ntohl(len);
	if (r != 4)
		throw DisconnectedException();
    received = ::recv(_sockfd, data, len, 0);
    if (received != len)
    {
		throw DisconnectedException();
	}
	data[received] = '\0';
	json = JSON::parse(data);
	delete[] data;
	return json;
}


bool net::TcpSocket::create()
{
    /* Initialization */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return isOpen();	// Socket created
}

void net::TcpSocket::close()
{
    if (isOpen())
    {
		::close(_sockfd);
	}
}

net::TcpSocket::TcpSocket()
{
	_sockfd = -1;
}

net::TcpSocket::~TcpSocket()
{
    close();
}
