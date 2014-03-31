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
	virtual void showSquad();
	virtual void showSelectPosition();
	virtual void showSelectPlayer();
	virtual void showSwapPlayer();
	virtual void onMatchStart();
	void showPlayerAttributes();
	bool isPlayerInTeam(int player_id)
	{
		for (size_t i = 0; i < user().players.size(); ++i)
		{
			if (user().players[i].getMemberID() == player_id)
				return true;
		}
		return false;
	}

	bool isPlayerInSquad(int player_id)
	{
		for (size_t i = 0; i < 7; ++i)
		{
			if (user().squad.players[i]->getMemberID() == player_id)
				return true;
		}
		return false;
	}
};

#endif // __CLI_TEAM_MANAGER_HPP