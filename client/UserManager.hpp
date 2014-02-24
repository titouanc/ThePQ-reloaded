#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <network/ClientConnectionManager.hpp>
#include <Constants.hpp>
#include "Menu.hpp"
#include "Exception.hpp"
#include "User.hpp"

class UserManager
{
public:
	UserManager(net::ClientConnectionManager& connection, User& user);
	
	bool displayMenu();
	
private:
	void doLoginMenu();
	void doRegisterMenu();
	
	void doLoginUser(std::string username, std::string password);
	void doRegisterUser(std::string username, std::string password);
	void doesUserExist(std::string username);
	
	net::ClientConnectionManager& _connection;
	User& _user;
	
};

#endif // __USER_MANAGER_HPP
