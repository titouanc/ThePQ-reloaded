#ifndef __CLI_USER_MANAGER_HPP
#define __CLI_USER_MANAGER_HPP

#include "UserManager.hpp"
#include <cli/CLI.hpp>

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

	void onLoginUser(std::string const & data);
	void onRegisterUser(std::string const & data);
	void onTeamName(std::string const & teamname);

private:
};

#endif // __CLI_USER_MANAGER_HPP