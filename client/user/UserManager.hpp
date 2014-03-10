#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <cli/Menu.hpp>
#include <Exception.hpp>
#include <ClientManager.hpp>
#include <pthread.h>

class UserManager : public ClientManager
{
protected:
	/* attempt to login */
	void loginUser(std::string username, std::string password);
	/* notify the server to logout */
	void logoutUser();
	/* attempt to register */
	void registerUser(std::string username, std::string password);
	/* unused for the moment : check if a user exists TODO */
	void doesUserExist(std::string username);
	/* choose a team name (on first login) */
	void chooseTeamName(std::string username, std::string teamname);
	/* Treat user specific messages */
	virtual void treatMessage(std::string const & type, JSON::Value const * data);
	
	/* Hooks */
	/* Triggered after successful login */
	virtual void onLoginOK(){}

	/* Triggered after login failed.
	   @param err_message: an error text */
	virtual void onLoginError(std::string const & err_message){}

	/* Triggered when server ask a new team name */
	virtual void onAskTeamName(){}

	/* Triggered after a successful register */
	virtual void onRegisterUserOK() {}
	/* Trigered when register fails */
	virtual void onRegisterUserError(std::string const & reason) {}

	/* Triggered when new team name is accepted */
	virtual void onTeamNameOK() {}
	/* Triggered when team name is rejected */
	virtual void onTeamNameError(std::string const & reason) {}
public:
	using ClientManager::ClientManager;
	UserManager(ClientManager const & parent);
};

#endif // __USER_MANAGER_HPP
