#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <network/ClientConnectionManager.hpp>
#include <Constants.hpp>
#include "Menu.hpp"
#include "Exception.hpp"

class UserManager
{
public:
	UserManager(net::ClientConnectionManager& connection);
	
	void displayMenu();
	
	std::string getUserName() { return _username; }
	
private:
	void doLogin();
	void doRegister();
	
	void doLoginUser(std::string username, std::string password);
	void doRegisterUser(std::string username, std::string password);
	void doesUserExist(std::string username);
	
	net::ClientConnectionManager _connection;
	std::string _username;
	
};

#endif // __USER_MANAGER_HPP
