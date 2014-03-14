#include "CLIUserManager.hpp"
#include <cli/Menu.hpp>
#include <stadium/CLIStadiumManager.hpp>
#include <team/CLITeamManager.hpp>
#include <market/CLIMarketManager.hpp>
#include <fgame/CLIFGameManager.hpp>
#include <championship/CLIChampionshipManager.hpp>
#include <match/CLIMatchManager.hpp>
#include <iostream>
#include <Constants.hpp>
using namespace std;

CLIUserManager::CLIUserManager(ClientManager const & parent) : 
UserManager(parent), _waitForNotificationResponse(false)
{}

void CLIUserManager::run()
{
	/* user menu */
	splashScreen();
	Menu _menu;
	_menu.addToDisplay("   - login\n");
	_menu.addToDisplay("   - register\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	_pending = 0;
	do {
		do {
			minisleep(0.1);
			readMessages();
		}
		while(_pending > 0);
		option = _menu.run();
		switch(option)
		{
			case 1:
				showLoginMenu();
				break;
			case 2:
				showRegisterMenu();
				break;
			default:
				break;
		}
	}
	while (option != 3);
	goodBye();
}

void CLIUserManager::showLoginMenu()
{	
	string username = Menu::askForUserData("Username : ");
	string password = Menu::askForUserData("Password : ");
	cout << "Please wait..." << endl;
	loginUser(username, password);
	_pending++;
}

void CLIUserManager::showTeamNameMenu()
{
	cout << "Hey, it's the first time you log in ! Please, pick up a name for your team." << endl;
	string teamname = Menu::askForUserData("Teamname : ");
	chooseTeamName(user().username,teamname);
	_pending++;
}

void CLIUserManager::showRegisterMenu()
{
	string username = Menu::askForUserData("Pick a username : ");
	// cout << "Please wait..." << endl;
	// doesUserExist(username);
	string password = "a";
	string passwordConfirmation;
	while (password != passwordConfirmation){
		password = Menu::askForUserData("Enter a new password : ");
		passwordConfirmation = Menu::askForUserData("Confirm password : ");
		if (password != passwordConfirmation)
			cout << "The two passwords entered were not the same." << endl;
	}
	cout << "Please wait..." << endl;
	registerUser(username, password);
	_pending++;
}

void CLIUserManager::showMainMenu()
{
	std::cout << "main menu ok"<< std::endl;
	readMessages();
	std::cout << "readmessages" <<std::endl;
	CLIMarketManager marketManager(*this);
	std::cout << "market ok"<<std::endl;
	CLIFGameManager friendlyGame(*this);
	std::cout << "fgame ok" << std::endl;
	CLIChampionshipManager champManager(*this);
	std::cout << "champ ok" << std::endl;
	Menu _menu;
	_menu.addToDisplay("   - manage your team and stadium\n");
	_menu.addToDisplay("   - access market\n");
	_menu.addToDisplay("   - play a friendly game\n");
	_menu.addToDisplay("   - access championships\n");
	_menu.addToDisplay("   - handle notifications\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				showManagementMenu();
				break;
			case 2:
				marketManager.run();
				break;
			case 3:
				friendlyGame.run();
				break;
			case 4:
				champManager.run();
				break;
			case 5:
				askForNotificationHandling();
				break;
			case 6:
				logoutUser();
			default:
				break;
		}
	}
	while (option != 6);
}

/* Management menu */
void CLIUserManager::showManagementMenu()
{
	CLIStadiumManager stadiumManager(*this);
	CLITeamManager teamManager(*this);
	Menu _menu;
	_menu.addToDisplay("   - manage your stadium and installations\n");
	_menu.addToDisplay("   - manage your players\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				stadiumManager.run();
				break;
			case 2:
				teamManager.run();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

/* HOOKS */
void CLIUserManager::onAskTeamName()
{
	_pending--;
	showTeamNameMenu();
}

void CLIUserManager::onLoginOK()
{
	_pending--;
	okMsg("Login successful");
	showMainMenu();
}

void CLIUserManager::onLoginError(std::string const & error)
{
	_pending--;
	errorMsg(error);
}

void CLIUserManager::onRegisterUserOK()
{
	_pending--;
	okMsg("You have successfully registered! You can now login.");
}

void CLIUserManager::onRegisterUserError(std::string const & data)
{
	_pending--;
	errorMsg("The username you chose already exists. Please pick another one");
	showRegisterMenu();
}

void CLIUserManager::onTeamNameOK()
{
	_pending--;
	okMsg("Team name accepted");
	showMainMenu();
}

void CLIUserManager::onTeamNameError(std::string const & reason)
{
	_pending--;
	errorMsg(reason);
	showTeamNameMenu();
}

void CLIUserManager::askForNotificationHandling()
{
	readMessages();
	Menu _menu;
	_menu.addToDisplay("   - handle this notification\n");
	_menu.addToDisplay("   - update list\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		int i =getNbNotifications();
		if (i>1){
			cout << "You have \033[33m" << getNbNotifications() << "\033[0m new notifications." << endl; 
		}else{
			cout << "You have \033[33m" << getNbNotifications() << "\033[0m new notification." << endl; 
		}
		option = _menu.run();
		switch(option)
		{
			case 1:
				handleNotification();
				break;
			case 2:
				readMessages();
				break;
			case 3:
				break;
			default:
				cout << "Wrong option entered" << endl;
				break;
		}
	}
	while (option!=3);
}

void CLIUserManager::onInvite(std::string const & user)
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
			onMatchStart();
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

void CLIUserManager::onMatchPending(){
	cout << "Your next championship match is close. What do you want to do ?" << endl;
	Menu _menu;
	_menu.addToDisplay("   - ready to play !\n");
	_menu.addToDisplay("   - withdraw from match (you will be \033[31msend off\033[0m championship)\n");
	int option;
	bool ok = false;
	do{
		option = _menu.run();
		if(option == 1){
			ok = true;
			readyForMatch();
		}
		else if(option == 2){
			ok = true;
			withdrawFromMatch();
		}
		else
			cout << "Wrong option entered"<<endl;
	}
	while(!ok);
	_waitForNotificationResponse = true;
	while(_waitForNotificationResponse)
		readMessage();
}

void CLIUserManager::onNotificationResponse(bool success, std::string const & response,std::string const & msg){
	//If opponent hasn't responded yet, wait until he does
	if(response != net::MSG::CHAMPIONSHIP_MATCH_WAIT)
		_waitForNotificationResponse = false;
	(success) ? okMsg(msg) : errorMsg(msg);
}

void CLIUserManager::onMessage(std::string const & message){
	cout << message << endl;
}

void CLIUserManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}
