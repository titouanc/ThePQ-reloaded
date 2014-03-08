#ifndef __CLI_TEAM_MANAGER_HPP
#define __CLI_TEAM_MANAGER_HPP

#include "TeamManager.hpp"
#include "CLI.hpp"

class CLITeamManager : public TeamManager, public CLI
{
public:
	using TeamManager::TeamManager;
	CLITeamManager(ClientManager const & parent);
	void run();
	virtual void showPlayers();
	virtual void onPlayersLoad(JSON::List const & players);
};

#endif // __CLI_TEAM_MANAGER_HPP