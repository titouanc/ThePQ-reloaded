#include "CLIFGameView.hpp"
#include <cli/CLI.hpp>
#include <match/CLIMatchView.hpp>

#include "graphics/MainController.hpp"
#include <match/GraphicMatchView.hpp>

#include <iostream>

using namespace std;
///Constructor
CLIFGameView::CLIFGameView(ClientController const & parent) : 
FGameController(parent)
{}

/**
 * Method handling invitation for players
 */
void CLIFGameView::showChooseUserMenu()
{
	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
	sendInvitation(userInput);
	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
}

/**
 * Method handling a friendly match display
 * @return : false if no game invitation
*/

bool CLIFGameView::showFriendlyMatchMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - list all connected players\n");
	_menu.addToDisplay("   - choose one to play a friendly game with\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	option = _menu.run();
	switch(option)
	{
		case 1:
			askConnectedList();
			_wait = true;
			break;
		case 2:
			showChooseUserMenu();
			_wait = true;
			break;
		default:
			return false;
	}
	return true;
}

/**
 * Method displaying connected users
 */
void CLIFGameView::onUserList(JSON::List const & list)
{
	cout << "\033[1mConnected users: \033[0m";
	for (size_t i=0; i<list.len(); i++){
		if (i > 0)
			cout << ", ";
		cout << STR(list[i]).value();
	}
	cout << endl;
	_wait = false;
}

/**
 * Method handling a friendly game
 */
void CLIFGameView::run()
{
	_wait = false;
	do{
		do {
			minisleep(0.1);
			readMessages();
		} while (_wait == true);
	}
	while(showFriendlyMatchMenu());
}

/**
 * Method handling accept token for friendly game
 * @param : string name of the player who accepted the game
 */
void CLIFGameView::onOtherAccept(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[32mhas accepted to play a friendly game !\033[0m" << endl;
	_wait = false;
	onMatchStart();
}

/**
 * Method handling token for friendly game
 * @param : string name of the player who denied the game
 */
void CLIFGameView::onOtherDeny(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[33mdoesn't want to play a friendly game now !\033[0m" << endl;
	_wait = false;
}

/**
 * Method handling inexisting user
 * @param : string name of the player (not found)
 */
void CLIFGameView::onUserNotFound(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[31mnot found !\033[0m" << endl;
	_wait = false;
}

/**
 * Method handling the start of a match
 */
void CLIFGameView::onMatchStart(){
	CLIMatchView match(*this); 
	match.run();
}
