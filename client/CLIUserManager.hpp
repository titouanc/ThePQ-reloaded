#ifndef __CLI_USER_MANAGER_HPP
#define __CLI_USER_MANAGER_HPP

#include "UserManager.hpp"
#include "CLIStadiumManager.hpp"

class CLIUserManager : public UserManager {
public:
	using UserManager::UserManager;
	CLIUserManager(ClientManager const & parent);
	virtual void run();
	void showLoginMenu();
	void showRegisterMenu();
	void showTeamNameMenu();
	void showMainMenu();
	void showManagementMenu();

private:
	CLIStadiumManager _stadiumManager;
};

#endif // __CLI_USER_MANAGER_HPP