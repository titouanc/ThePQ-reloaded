#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "Message.hpp"
#include "Connection.hpp"


class Client
{
public:
	Client();
	~Client();
	
	void run();

private:
	std::string _userChoice;

};

#endif // __CLIENT_HPP
