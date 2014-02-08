#include "net.hpp"
#include <cstring>

net::Socket::Status net::TcpSocket::connect(const std::string ipAddr, int portNo)
{
    close();
    if (!create())
    {
		return Status::ERROR;
	}

    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    _servAddr.sin_port = htons(portNo);

    if (::connect(_sockfd, (struct sockaddr *) &_servAddr,
		sizeof(_servAddr)) < 0)
	{
		return Status::ERROR;
	}
    return Status::OK;
}

net::Socket::Status net::TcpSocket::send(const char *data, size_t len)
{
    if (::send(_sockfd, data, len, 0) <= 0)
    {
		return Status::ERROR;
	}
    return Status::OK;
}

net::Socket::Status net::TcpSocket::recv(char *data, size_t len, size_t & received)
{
	data = new char[100];
	memset(data, 0, 100);
    received = ::recv(_sockfd, data, len, 0);
    if (received <= 0)
    {
		return Status::ERROR;
	}
	data[received] = '\0';
    return Status::OK;
}

net::Socket::Status net::TcpSocket::send(const JSON::Value *json)
{
	std::string dump = json->dumps();
	const char* data = (dump.c_str());
    if (::send(_sockfd, data, dump.length(), 0) <= 0)
    {
		return Status::ERROR;
	}
    return Status::OK;
}

net::Socket::Status net::TcpSocket::recv(JSON::Value **json)
{
	char* data = new char[100];
	size_t received;
	bzero(data, 100);
    received = ::recv(_sockfd, data, 100, 0);
    if (received <= 0)
    {
		return Status::ERROR;
	}
	data[received] = '\0';
	*json = JSON::parse(data);
	delete data;
	return Status::OK;
}
