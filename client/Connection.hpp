#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "net.hpp"
#include "json.h"

class Connection
{
public:

	
	static bool isLogged();
	
private:
	static bool _isLogged;
	
};

#endif
