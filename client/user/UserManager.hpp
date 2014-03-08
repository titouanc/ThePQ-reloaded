#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include <string>
#include <Constants.hpp>
#include <cli/Menu.hpp>
#include <Exception.hpp>
#include <ClientManager.hpp>

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
	/* received login user response */
	virtual void onLoginUser(std::string const &){}
	/* received register user response */
	virtual void onRegisterUser(std::string const &) {}
	/* received team name choice response */
	virtual void onTeamName(std::string const &) {}
public:
	using ClientManager::ClientManager;
	UserManager(ClientManager const & parent);
};

#endif // __USER_MANAGER_HPP
