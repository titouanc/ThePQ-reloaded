#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <network/TcpSocket.hpp>
#include <json/json.hpp>
#include "Exception.hpp"
#include <Constants.hpp>
#include <string>
#include <vector>
#include "model/Installation.hpp"

class Connection
{
public:
	Connection();
	
	void loginUser(std::string username, std::string passwd);
	void doesUserExist(std::string username);
	void registerUser(std::string username, std::string passwd);
	
	std::vector<Installation> getInstallationsList();
	void getConnectedUsersList(std::vector<std::string> &users);
	
private:
	net::TcpSocket _socket;
};

#endif
