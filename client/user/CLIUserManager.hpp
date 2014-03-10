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

	void onAskTeamName();

	void onLoginOK();
	void onLoginError(std::string const & error);

	void onTeamNameOK();
	void onTeamNameError(std::string const & error);

	void onRegisterUserOK();
	void onRegisterUserError(std::string const & data);

	void askForNotificationHandling();
	void onInvite(std::string const & username);
	void onMessage(std::string const & message);
	void onMatchStart();

private:
};

#endif // __CLI_USER_MANAGER_HPP