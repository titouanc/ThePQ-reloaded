#include "CLITeamManager.hpp"
#include <match/CLIMatchManager.hpp>

using namespace SQUAD;

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
		showPlayers();
		showSquad();
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_pending > 0);
		option = _menu.run();
		switch(option)
		{
			case 1:
				showSelectPosition();
				break;
			case 2:
				showSelectPlayer();
			default:
				break;
		}
	}
	while (option != 2);
}

void CLITeamManager::showPlayers(){
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl;
	}
	cout << "==============================================" << endl;
}

void CLITeamManager::showSquad()
{
	cout << "================ YOUR SQUAD ==================" << endl;
	cout << LEFT_CHASER << " - \e[34mLeft Chaser\e[0m : 		" << user().squad.players[LEFT_CHASER]->getName() << endl;
	cout << CENTER_CHASER << " - \e[34mCenter Chaser\e[0m : 		" << user().squad.players[CENTER_CHASER]->getName() << endl;
	cout << RIGHT_CHASER << " - \e[34mRight Chaser\e[0m : 		" << user().squad.players[RIGHT_CHASER]->getName() << endl;
	cout << LEFT_BEATER << " - \e[34mLeft Beater\e[0m : 		" << user().squad.players[LEFT_BEATER]->getName() << endl;
	cout << RIGHT_BEATER << " - \e[34mRight Beater\e[0m : 		" << user().squad.players[RIGHT_BEATER]->getName() << endl;
	cout << SEEKER << " - \e[34mSeeker\e[0m : 			" << user().squad.players[SEEKER]->getName() << endl;
	cout << KEEPER << " - \e[34mKeeper\e[0m : 			" << user().squad.players[KEEPER]->getName() << endl;
	if (getSelectedPosition() > -1)
		cout << "Current selected position is " << getSelectedPosition() << endl;
	cout << "==============================================" << endl;
}

void CLITeamManager::showSelectPosition()
{

}

void CLITeamManager::showSelectPlayer()
{

}

void CLITeamManager::onPlayersLoad()
{
	_pending--;
}

void CLITeamManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}