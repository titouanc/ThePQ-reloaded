#include "CLIFGameManager.hpp"
#include <cli/CLI.hpp>
#include <match/CLIMatchManager.hpp>

#include "graphics/MainController.hpp"
#include <match/GraphicMatchManager.hpp>

#include <iostream>

using namespace std;
///Constructor
CLIFGameManager::CLIFGameManager(ClientManager const & parent) : 
FGameManager(parent)
{}

/**
 * Method handling invitation for players
 */
void CLIFGameManager::showChooseUserMenu()
{
	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
	sendInvitation(userInput);
	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
}

/**
 * Method handling a friendly match display
 * @return : false if no game invitation
*/

bool CLIFGameManager::showFriendlyMatchMenu()
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
			_pending++;
			break;
		case 2:
			showChooseUserMenu();
			_pending++;
			break;
		default:
			return false;
	}
	return true;
}

/**
 * Method displaying connected users
 */
void CLIFGameManager::onUserList(JSON::List const & list)
{
	cout << "\033[1mConnected users: \033[0m";
	for (size_t i=0; i<list.len(); i++){
		if (i > 0)
			cout << ", ";
		cout << STR(list[i]).value();
	}
	cout << endl;
	_pending--;
}

/**
 * Method handling a friendly game
 */
void CLIFGameManager::run()
{
	_pending = 0;
	do{
		do {
			minisleep(0.1);
			readMessages();
		} while (_pending > 0);
	}
	while(showFriendlyMatchMenu());
}

/**
 * Method handling accept token for friendly game
 * @param : string name of the player who accepted the game
 */
void CLIFGameManager::onOtherAccept(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[32mhas accepted to play a friendly game !\033[0m" << endl;
	_pending--;
	onMatchStart();
}

/**
 * Method handling token for friendly game
 * @param : string name of the player who denied the game
 */
void CLIFGameManager::onOtherDeny(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[33mdoesn't want to play a friendly game now !\033[0m" << endl;
	_pending--;
}

/**
 * Method handling inexisting user
 * @param : string name of the player (not found)
 */
void CLIFGameManager::onUserNotFound(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[31mnot found !\033[0m" << endl;
	_pending--;
}

/**
 * Method handling the start of a match
 */
void CLIFGameManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}
