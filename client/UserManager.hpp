#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <network/ClientConnectionManager.hpp>
#include <Constants.hpp>
#include "Menu.hpp"
#include "Exception.hpp"
#include "UserData.hpp"

class UserManager
{
public:
	UserManager(net::ClientConnectionManager& connection, UserData& user);
	
	bool showMenu();
	
protected:
	void doLoginMenu();
	void doRegisterMenu();
	
	void doLoginUser(std::string username, std::string password);
	void doRegisterUser(std::string username, std::string password);
	void doesUserExist(std::string username);

	net::ClientConnectionManager& _connection;
	UserData& _user;
	
};

#endif // __USER_MANAGER_HPP
