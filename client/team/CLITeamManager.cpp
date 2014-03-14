#include "CLITeamManager.hpp"
#include <match/CLIMatchManager.hpp>

using namespace SQUAD;

CLITeamManager::CLITeamManager(ClientManager const & parent) : TeamManager(parent)
{}

void CLITeamManager::run()
{
	Menu _menu;
	_menu.addToDisplay("	- Put player at the selected position");
	_menu.addToDisplay("	- Swap player at the selected position with another player");
	_menu.addToDisplay("	- Quit to management menu\n");
	int option;
	_pending = 0;
	do
	{
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_pending > 0);
		showPlayers();
		showSquad();
		option = _menu.run();
		switch(option)
		{
			case 1:
				showSelectPlayer();
				break;
			case 2:
				showSwapPlayer();
			default:
				break;
		}
	}
	while (option != 3);
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
	cout << LEFT_CHASER << " - \e[34mLeft Chaser\e[0m : 		" << user().squad.players[LEFT_CHASER]->getName() << " - ID: " << user().squad.players[LEFT_CHASER]->getMemberID() << endl;
	cout << CENTER_CHASER << " - \e[34mCenter Chaser\e[0m : 		" << user().squad.players[CENTER_CHASER]->getName() << " - ID: " << user().squad.players[CENTER_CHASER]->getMemberID() << endl;
	cout << RIGHT_CHASER << " - \e[34mRight Chaser\e[0m : 		" << user().squad.players[RIGHT_CHASER]->getName() << " - ID: " << user().squad.players[RIGHT_CHASER]->getMemberID() << endl;
	cout << LEFT_BEATER << " - \e[34mLeft Beater\e[0m : 		" << user().squad.players[LEFT_BEATER]->getName() << " - ID: " << user().squad.players[LEFT_BEATER]->getMemberID() << endl;
	cout << RIGHT_BEATER << " - \e[34mRight Beater\e[0m : 		" << user().squad.players[RIGHT_BEATER]->getName() << " - ID: " << user().squad.players[RIGHT_BEATER]->getMemberID() << endl;
	cout << SEEKER << " - \e[34mSeeker\e[0m : 			" << user().squad.players[SEEKER]->getName() << " - ID: " << user().squad.players[SEEKER]->getMemberID() << endl;
	cout << KEEPER << " - \e[34mKeeper\e[0m : 			" << user().squad.players[KEEPER]->getName() << " - ID: " << user().squad.players[KEEPER]->getMemberID() << endl;
	cout << "==============================================" << endl;
}

void CLITeamManager::showSelectPosition()
{
	int position;
	do
	{
		cout << "Choose a position (number)" << endl << ">";
		cin >> position;
	}
	while (position < 0 || position > 6);
	selectPosition(position);

}

void CLITeamManager::showSelectPlayer()
{
	int player_id;
	do
	{
		cout << "Choose the player (ID) you want to add in your squad (-1 to quit)" << endl << ">";
		cin >> player_id;
		if (player_id == -1) return;
	}
	while(!isPlayerInTeam(player_id) || isPlayerInSquad(player_id));
	showSelectPosition();
	putPlayerAtPosition(player_id, getSelectedPosition());
}

void CLITeamManager::showSwapPlayer()
{
	int player_id;
	do
	{
		cout << "Choose the player (ID) you want to swap" << endl << ">";
		cin >> player_id;
	}
	while(!isPlayerInSquad(player_id));
	showSelectPosition();
	swapPlayers(player_id, getSelectedPosition());
}

void CLITeamManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}