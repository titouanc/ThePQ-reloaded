#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "Messages.hpp"
#include "Connection.hpp"


class Client
{
public:
	Client();
	~Client();
	
	void run();

private:
	net::TcpSocket _socket;
	std::string _userChoice;

};

#endif // __CLIENT_HPP
