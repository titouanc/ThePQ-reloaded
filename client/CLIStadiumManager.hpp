#ifndef __CLI_STADIUM_MANAGER_HPP
#define __CLI_STADIUM_MANAGER_HPP

#include "StadiumManager.hpp"
#include "CLI.hpp"

class CLIStadiumManager : public StadiumManager, public CLI {
public:
	using StadiumManager::StadiumManager;
	CLIStadiumManager(ClientManager const & parent);
	virtual void run();

private:
	void printInstallationsList();
	void showUpgradeInstallation();
	void showDowngradeInstallation();
	virtual void treatMessage(std::string const & type, JSON::Value const * data);
};

#endif // __CLI_STADIUM_MANAGER_HPP