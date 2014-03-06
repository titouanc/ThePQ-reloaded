#ifndef __CLI_USER_MANAGER_HPP
#define __CLI_USER_MANAGER_HPP

#include "UserManager.hpp"

class CLIUserManager : public UserManager {
public:
	using UserManager::UserManager;
	virtual void run();
	void showLoginMenu();
	void showRegisterMenu();
	void showTeamNameMenu();

private:

};

#endif // __CLI_USER_MANAGER_HPP