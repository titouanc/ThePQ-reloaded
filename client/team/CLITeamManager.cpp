#include "CLITeamManager.hpp"
#include <match/CLIMatchView.hpp>

using namespace SQUAD;

/// Constructor
CLITeamManager::CLITeamManager(ClientController const & parent) : TeamManager(parent)
{}

/**
 * Method handling the team management interface
 */
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
				cout << "\033[2J\033[1;1H";
				showSelectPlayer();
				break;
			case 2:
				cout << "\033[2J\033[1;1H";
				showSwapPlayer();
			default:
				break;
		}
	}
	while (option != 3);
}

/**
 * Method displaying the teams players
 */
void CLITeamManager::showPlayers(){
	cout << "\033[2J\033[1;1H";
	cout << "		================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout <<"	"<< user().players[i] << endl;
	}
	cout << "		==============================================" << endl;
}

/**
 * Method displaying the users squad
 */
void CLITeamManager::showSquad()
{
	cout << "\033[2J\033[1;1H";
	cout << "================ YOUR SQUAD ==================" << endl;
	cout << LEFT_CHASER << " - \033[34mLeft Chaser\033[0m : 		" << user().squad.players[LEFT_CHASER]->getName() << " - ID: " << user().squad.players[LEFT_CHASER]->getMemberID() << endl;
	cout << CENTER_CHASER << " - \033[34mCenter Chaser\033[0m : 		" << user().squad.players[CENTER_CHASER]->getName() << " - ID: " << user().squad.players[CENTER_CHASER]->getMemberID() << endl;
	cout << RIGHT_CHASER << " - \033[34mRight Chaser\033[0m : 		" << user().squad.players[RIGHT_CHASER]->getName() << " - ID: " << user().squad.players[RIGHT_CHASER]->getMemberID() << endl;
	cout << LEFT_BEATER << " - \033[34mLeft Beater\033[0m : 		" << user().squad.players[LEFT_BEATER]->getName() << " - ID: " << user().squad.players[LEFT_BEATER]->getMemberID() << endl;
	cout << RIGHT_BEATER << " - \033[34mRight Beater\033[0m : 		" << user().squad.players[RIGHT_BEATER]->getName() << " - ID: " << user().squad.players[RIGHT_BEATER]->getMemberID() << endl;
	cout << SEEKER << " - \033[34mSeeker\033[0m : 			" << user().squad.players[SEEKER]->getName() << " - ID: " << user().squad.players[SEEKER]->getMemberID() << endl;
	cout << KEEPER << " - \033[34mKeeper\033[0m : 			" << user().squad.players[KEEPER]->getName() << " - ID: " << user().squad.players[KEEPER]->getMemberID() << endl;
	cout << "==============================================" << endl;
}

/**
 * Method handling user input for a players position
 */
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

/**
 * Method handling player position on pitch
 */
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

/**
 * Method handling a player swap (squad <-> team)
 */
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

/**
 * Method handling the match start
 */
void CLITeamManager::onMatchStart(){
	CLIMatchView match(*this); 
	match.run();
}