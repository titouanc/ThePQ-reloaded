#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "net.hpp"
#include "json.h"

class Connection
{
public:
	Connection();
	~Connection();
	
	bool isLogged();
	
private:
	bool _isLogged;
	
	net::TcpSocket _socket;
	
};

#endif
