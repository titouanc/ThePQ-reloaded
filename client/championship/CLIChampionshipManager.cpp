#include "CLIChampionshipManager.hpp"

void run(){
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

void joinChampionshipMenu(){
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

void currentChampionshipMenu(){
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



void displayCurrentChampionship(){
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

void displayChampionships(){
	_waitForChamps = true;
	loadChampionships();
	while (_waitForChamps)
		readMessage();
	std::cout << "================ CHAMPIONSHIPS ================" << std::endl;
	for(size_t i=0;i<getChamps().size();++i){
		std::cout<<getChamps()[i]<<std::endl;
	}
	std::cout << "===============================================" << std::endl;
}

void onJoinChampionship(std::string data){
	_waitForJoin = false;
	if(data == net::MSG::ALREADY_IN_CHAMPIONSHIP){
		std::cout << "\033[1;31mError\033[0m : cannot take part in more than one championship at the same time." << std::endl;
	}
	else if(data == net::MSG::CHAMPIONSHIP_FULL){
		std::cout << "\033[1;31mError\033[0m : the championship you tried to join is now full." << std::endl;
	}
}

void onLeaveChampionship(std::string data){
	_waitForJoin = false;
	if(data == net::MSG::CHAMPIONSHIP_STARTED){
		std::cout << "\033[1;31mError\033[0m : your championship started; cannot leave it." << std::endl;
	}
	else if(data == net::MSG::NOT_IN_CHAMPIONSHIP){
		std::cout << "\033[1;31mError\033[0m : you are currently not taking part in any championship; cannot leave any." << std::endl;
	}
}

void onChampionshipsLoad(JSON::List const & list){
	_waitForChamps = false;
	ChampionshipManager::onChampionshipsLoad(list);
}