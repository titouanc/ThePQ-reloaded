#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "../common/network/net.hpp"
#include "../common/json/json.hpp"
#include "Exception.hpp"
#include "../common/Constants.hpp"
#include <string>

class Connection
{
public:
	Connection();
	
	void loginUser(std::string username, std::string passwd);
	void doesUserExist(std::string username);
	void registerUser(std::string username, std::string passwd);
	
	
private:
	net::TcpSocket _socket;
};

#endif
