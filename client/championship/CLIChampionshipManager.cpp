#include "CLIChampionshipManager.hpp"

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
		std::cout << "Championship name not correct." << std::endl;
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
	loadChampionships();
	while (_waitForChamps)
		readMessage();
	updateCurrentChampionship();
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
		std::cout<< getChamps()[i];
	}
	std::cout << "===============================================" << std::endl;
}

void CLIChampionshipManager::onJoinChampionship(std::string data){
	_waitForJoin = false;
	if(data == net::MSG::ALREADY_IN_CHAMPIONSHIP){
		std::cout << "\033[1;31mError\033[0m : cannot take part in more than one championship at the same time." << std::endl;
	}
	else if(data == net::MSG::CHAMPIONSHIP_FULL){
		std::cout << "\033[1;31mError\033[0m : the championship you tried to join is now full." << std::endl;
	}
	else if(data == net::MSG::CHAMPIONSHIP_NOT_FOUND){
		std::cout << "\033[1;31mError\033[0m : championship not found." << std::endl;
	}
	else if(data == net::MSG::ADDED_TO_CHAMPIONSHIP){
		std::cout << "\033[32mSuccessfully\033[0m added to championship." << std::endl;
	}
}

void CLIChampionshipManager::onLeaveChampionship(std::string data){
	_waitForJoin = false;
	if(data == net::MSG::CHAMPIONSHIP_STARTED){
		std::cout << "\033[1;31mError\033[0m : your championship started; cannot leave it." << std::endl;
	}
	else if(data == net::MSG::NOT_IN_CHAMPIONSHIP){
		std::cout << "\033[1;31mError\033[0m : you are currently not taking part in any championship; cannot leave any." << std::endl;
	}
	else if(data == net::MSG::REMOVED_FROM_CHAMPIONSHIP){
		std::cout << "\033[32mSuccessfully\033[0m removed from championship." << std::endl;
	}
}

void CLIChampionshipManager::onChampionshipsLoad(JSON::List const & list){
	_waitForChamps = false;
	ChampionshipManager::onChampionshipsLoad(list);
}