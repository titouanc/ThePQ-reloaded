#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "net.hpp"
#include "json.h"
#include "Exception.hpp"
#include <string>

class Connection
{
public:
	Connection();
	
	void loginUser(std::string username, std::string passwd);
	void registerUser(std::string username, std::string passwd);
	
	bool isLogged();
	
private:
	bool _isLogged;
	net::TcpSocket _socket;
};

#endif
