#include "CLITeamManager.hpp"
#include <match/CLIMatchManager.hpp>

CLITeamManager::CLITeamManager(ClientManager const & parent) : TeamManager(parent)
{}

void CLITeamManager::run()
{
	Menu _menu;
	_menu.addToDisplay("	- Show list of players");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	_pending = 0;
	loadPlayers();
	_pending++;
	do
	{
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_pending > 0);
		option = _menu.run();
		switch(option)
		{
			case 1:
				showPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 2);
}

void CLITeamManager::showPlayers(){
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

void CLITeamManager::onPlayersLoad(JSON::List const & players)
{
	TeamManager::onPlayersLoad(players);
	_pending--;
}

void CLITeamManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}