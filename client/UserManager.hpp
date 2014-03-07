#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <Constants.hpp>
#include "Menu.hpp"
#include "Exception.hpp"
#include "ClientManager.hpp"

class UserManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	UserManager(ClientManager const & parent);
	
	void loginUser(std::string username, std::string password);
	void logoutUser();
	void registerUser(std::string username, std::string password);
	void doesUserExist(std::string username);
	void chooseTeamName(std::string username, std::string teamname);

	virtual void treatMessage(std::string const & type, JSON::Value const * data);
	/* Hooks */
	virtual void onLoginUser(std::string){}
	virtual void onRegisterUser(std::string) {}
	virtual void onTeamName(std::string) {}
};

#endif // __USER_MANAGER_HPP
