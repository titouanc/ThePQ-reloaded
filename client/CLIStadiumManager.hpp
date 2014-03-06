#ifndef __CLI_STADIUM_MANAGER_HPP
#define __CLI_STADIUM_MANAGER_HPP

#include "StadiumManager.hpp"

class CLIStadiumManager : public StadiumManager {
public:
	using StadiumManager::StadiumManager;
	CLIStadiumManager(ClientManager const & parent);
	virtual void run();

private:
	void printInstallationsList();
	void upgradeInstallation();
	void downgradeInstallation();

};

#endif // __CLI_STADIUM_MANAGER_HPP