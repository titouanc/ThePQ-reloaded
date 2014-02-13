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
	
	void getInstallationsList(JSON::Dict & toFill);
	void Connection::getConnectedUsersList(vector<string> users)
	
private:
	net::TcpSocket _socket;
};

#endif
