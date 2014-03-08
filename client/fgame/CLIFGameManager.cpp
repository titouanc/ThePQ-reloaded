#include "CLIFGameManager.hpp"
#include <cli/CLI.hpp>
#include <match/CLIMatchManager.hpp>

#include "graphics/MainController.hpp"
#include "GraphicMatchManager.hpp"

#include <iostream>

using namespace std;

CLIFGameManager::CLIFGameManager(ClientManager const & parent) : 
FGameManager(parent)
{}

void CLIFGameManager::showChooseUserMenu()
{
	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
	sendInvitation(userInput);
	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
}

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

void startGUIMatch(ClientManager const & client)
{
	GUI::MainController Gcontroller;
	GraphicMatchManager match(client, Gcontroller);
	match.run();
}


void CLIFGameManager::onOtherAccept(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[32mhas accepted to play a friendly game !\033[0m" << endl;
	_pending--;
	/*CLIMatchManager match(*this);
	match.run();*/
	startGUIMatch(*this);
}

void CLIFGameManager::onOtherDeny(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[33mdoesn't want to play a friendly game now !\033[0m" << endl;
	_pending--;
}

void CLIFGameManager::onUserNotFound(std::string const & name)
{
	cout << "\033[1m" << name 
		 << " \033[31mnot found !\033[0m" << endl;
	_pending--;
}

void CLIFGameManager::onInvite(std::string const & user)
{
	cout << user << " invited you to a game" << endl;
	Menu _menu;
	_menu.addToDisplay("   - accept\n");
	_menu.addToDisplay("   - deny\n");
	int option;
	bool ok = false;
	do
	{
		option = _menu.run();
		if (option == 1){
			ok = true;
			
			acceptInvitationFromUser(user);
			//CLIMatchManager match(*this); 
			//match.run();
			
			startGUIMatch(*this);

			break;
		} else if (option == 2){
			ok = true;
			denyInvitationFromUser(user);
			break;
		} else {
			cout << "Wrong option entered" << endl;
			break;
		}
	}
	while(! ok);
}