#include "TcpSocket.hpp"
#include <cstring>
#include <sstream>

void net::TcpSocket::send(JSON::Value const & json)
{
	std::string dump = json.dumps();
	const char* data = dump.c_str();
	uint32_t len = htonl(dump.length());
	int r = ::send (_sockfd, &len, 4, 0);
	if (r != 4)
		throw DisconnectedException();
	
	for (size_t i=0; i<dump.length(); i+=r){
		r = ::send(_sockfd, data+i, dump.length()-i, 0);
		if (r <= 0){
			throw DisconnectedException();
		}
	}
}

void net::TcpSocket::loop()
{
	while (_isRunning == true)
	{
		std::stringstream res;
		char data[MSG_SIZE+1];
		bzero(data, MSG_SIZE);
		uint32_t len = 0;
		int r = ::recv(_sockfd, &len, 4, 0);
		len = ntohl(len);
		if (r != 4)
		{
			throw DisconnectedException();
		}
		
		
		while (len > 0) {
			r = recv(_sockfd, data, MSG_SIZE, 0);
			if (r == 0) throw DisconnectedException();
			data[r] = '\0';
			res << data;
			len -= r;
		}
		JSON::Value* json = JSON::parse(res.str().c_str());
		if (json != NULL)
			_messages.push(json);
	}
}

void net::TcpSocket::start()
{
	_isRunning = true;
	loop();
}

void net::TcpSocket::stop()
{
	_isRunning = false;
}

net::TcpSocket::TcpSocket(const std::string hostname, int portno) : 
	_isRunning(false), _sockfd(-1)
{
	struct hostent *host = NULL;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
		throw ConnectionFailedException();
    
    host = gethostbyname(hostname.c_str());
    if (host == NULL)
		throw ConnectionFailedException();
    
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr = *((struct in_addr*) *(host->h_addr_list));
    _servAddr.sin_port = htons(portno);

    if (::connect(_sockfd, (struct sockaddr *) &_servAddr,
		sizeof(_servAddr)) < 0)
	{
		throw ConnectionFailedException();
	}
}

net::TcpSocket::~TcpSocket()
{
	::close(_sockfd);
}
