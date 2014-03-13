#ifndef __CLI_TEAM_MANAGER_HPP
#define __CLI_TEAM_MANAGER_HPP

#include "TeamManager.hpp"
#include <cli/CLI.hpp>

class CLITeamManager : public TeamManager, public CLI
{
public:
	using TeamManager::TeamManager;
	CLITeamManager(ClientManager const & parent);
	void run();
	virtual void showPlayers();
	virtual void onPlayersLoad();
	virtual void onMatchStart();
};

#endif // __CLI_TEAM_MANAGER_HPP