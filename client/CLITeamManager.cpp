#include "CLITeamManager.hpp"

CLITeamManager::CLITeamManager(ClientManager const & parent) : TeamManager(parent)
{}

void CLITeamManager::run()
{
	Menu _menu;
	_menu.addToDisplay("	- Show list of players");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				displayPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 2);
}

void CLITeamManager::displayPlayers(){
	loadPlayers();
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}