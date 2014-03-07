#ifndef __CLI_USER_MANAGER_HPP
#define __CLI_USER_MANAGER_HPP

#include "UserManager.hpp"
#include "CLI.hpp"

class CLIUserManager : public UserManager, public CLI {
public:
	using UserManager::UserManager;
	CLIUserManager(ClientManager const & parent);
	virtual void run();
	void showLoginMenu();
	void showRegisterMenu();
	void showTeamNameMenu();
	void showMainMenu();
	void showManagementMenu();

	void onLoginUser(std::string data);
	void onRegisterUser(std::string data);
	void onTeamName(std::string data);

private:
};

#endif // __CLI_USER_MANAGER_HPP