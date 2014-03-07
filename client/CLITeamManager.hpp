#ifndef __CLI_TEAM_MANAGER_HPP
#define __CLI_TEAM_MANAGER_HPP

#include "TeamManager.hpp"

class CLITeamManager : public TeamManager
{
public:
	using TeamManager::TeamManager;
	CLITeamManager(ClientManager const & parent);
	void run();
	void displayPlayers();
};

#endif // __CLI_TEAM_MANAGER_HPP