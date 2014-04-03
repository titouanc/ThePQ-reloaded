#ifndef __CLI_STADIUM_MANAGER_HPP
#define __CLI_STADIUM_MANAGER_HPP

#include "StadiumController.hpp"
#include <cli/CLI.hpp>


class CLIStadiumView : public StadiumController, public CLI {
public:
	using StadiumController::StadiumController;
	CLIStadiumView(ClientController const & parent);
	virtual void run();

private:
	void printInstallationsList();
	void showUpgradeInstallation();
	void showDowngradeInstallation();
	virtual void onMatchStart();
	virtual void treatMessage(std::string const & type, JSON::Value const * data);
};

#endif // __CLI_STADIUM_MANAGER_HPP