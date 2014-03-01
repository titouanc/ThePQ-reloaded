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
	
	void loginUser(std::string username, std::string password);
	void registerUser(std::string username, std::string password);
	void doesUserExist(std::string username);

private:
	net::ClientConnectionManager& _connection;
	UserData& _user;
	
};

#endif // __USER_MANAGER_HPP
