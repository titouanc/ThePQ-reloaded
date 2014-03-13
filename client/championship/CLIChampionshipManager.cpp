#include "CLIChampionshipManager.hpp"
#include <match/CLIMatchManager.hpp>

CLIChampionshipManager::CLIChampionshipManager(ClientManager const & parent) : 
ChampionshipManager(parent), _waitForChamps(false), _waitForJoin(false)
{}

void CLIChampionshipManager::run(){
	Menu _menu;
	_menu.addToDisplay("   - join a championship\n");
	_menu.addToDisplay("   - access your championship\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do {
		readMessages();
		option = _menu.run();
		switch(option)
		{
			case 1:
				joinChampionshipMenu();
				break;
			case 2:
				currentChampionshipMenu();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

void CLIChampionshipManager::joinChampionshipMenu(){
	displayChampionships();
	std::cout << "Enter the name of the championship you wish to join : ";
	std::string champName;
	while(champName.empty())
		std::getline(cin,champName);
	bool found = false;
	for(size_t i = 0;i<getChamps().size();++i){
		if (getChamps()[i].getName() == champName){
			found = true;
		}
	}
	if(found){
		_waitForJoin = true;
		joinChampionship(champName);
		while (_waitForJoin)
			readMessage();
	}
	else
		std::cout << "\033[1;31mError\033[0m : championship name not correct." << std::endl;
}

void CLIChampionshipManager::currentChampionshipMenu(){
	displayCurrentChampionship();
	Menu _menu;
	_menu.addToDisplay("   - leave your championship\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	_pending = 0;
	do {
		readMessages();
		option = _menu.run();
		switch(option)
		{
			case 1:
				_waitForJoin = true;
				leaveCurrentChampionship();
				while(_waitForJoin)
					readMessage();
				break;
			default:
				break;
		}
	}
	while(option != 2);
}

void CLIChampionshipManager::displayCurrentChampionship(){
	_waitForChamps = true;
	joinedChampionship();
	while (_waitForChamps)
		readMessage();
	if(user().joinedChamp.getName().empty()){
		std::cout << "You are currently not taking part in any championship." << std::endl;
	}
	else
		std::cout << "Your championship :\n" << user().joinedChamp << std::endl;
}

void CLIChampionshipManager::displayChampionships(){
	_waitForChamps = true;
	loadChampionships();
	while (_waitForChamps)
		readMessage();
	std::cout << "================ CHAMPIONSHIPS ================" << std::endl;
	for(size_t i=0;i<getChamps().size();++i){
		std::cout<< getChamps()[i] << endl;
	}
	std::cout << "===============================================" << std::endl;
}

void CLIChampionshipManager::onJoinChampionship(bool success, std::string msg){
	_waitForJoin = false;
	(success) ? okMsg(msg) : errorMsg(msg);
	
}

void CLIChampionshipManager::onLeaveChampionship(bool success, std::string msg){
	_waitForJoin = false;
	(success) ? okMsg(msg) : errorMsg(msg);
}

void CLIChampionshipManager::onChampionshipsLoad(){
	_waitForChamps = false;
}

void CLIChampionshipManager::onJoinedChampionship(){
	_waitForChamps = false;
}

void CLIChampionshipManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}