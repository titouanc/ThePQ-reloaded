#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "net.hpp"
#include "Messages.hpp"
#include "Connexion.hpp"
#include "../common/Constants.hpp"
#include "Exception.hpp"

using namespace std;

class Client
{
public:
	Client();
	~Client();
	
	void run();

private:
	net::TcpSocket _socket;

};

#endif // __CLIENT_HPP
