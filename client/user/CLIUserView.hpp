#ifndef __CLI_USER_MANAGER_HPP
#define __CLI_USER_MANAGER_HPP

#include "UserController.hpp"
#include <cli/CLI.hpp>

class CLIUserView : public UserController, public CLI {
public:
	using UserController::UserController;
	CLIUserView(ClientController const & parent);
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
	void onMatchPending();
	void onNotificationResponse(bool,std::string const&,std::string const &);

private:
	bool _waitForNotificationResponse;
};

#endif // __CLI_USER_MANAGER_HPP