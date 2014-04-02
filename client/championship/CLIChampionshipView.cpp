#include "CLIChampionshipView.hpp"
#include <match/CLIMatchManager.hpp>

///Constructor
CLIChampionshipView::CLIChampionshipView(ClientController const & parent) : 
ChampionshipController(parent), _waitForChamps(false), _waitForJoin(false)
{}
/**
 * Method starting the championship interface
 */
void CLIChampionshipView::run(){
	
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

/**
 * Method handling a championship inscription
 */
void CLIChampionshipView::joinChampionshipMenu(){
	
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

/**
 * Method displaying possible actions in the current championship
 */
void CLIChampionshipView::currentChampionshipMenu(){
	
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

/**
 * Method displaying the current state of the championship (users,teams...)
 */
void CLIChampionshipView::displayCurrentChampionship(){
	
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

/**
 * Method listing all available championships
 */
void CLIChampionshipView::displayChampionships(){
	
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

/**
 * Method handling the entry in a championship
 */
void CLIChampionshipView::onJoinChampionship(bool success, const std::string & msg){
	
	_waitForJoin = false;
	(success) ? okMsg(msg) : errorMsg(msg);
	
}

/**
 * Method handling the unsubscription from a championship
 */
void CLIChampionshipView::onLeaveChampionship(bool success, const std::string & msg){
	
	_waitForJoin = false;
	(success) ? okMsg(msg) : errorMsg(msg);
}

/**
 * Method setting the flag not to wait for a championship if
 * the championship is being loaded
 */
void CLIChampionshipView::onChampionshipsLoad(){
	_waitForChamps = false;
}

/**
 * Method setting the flag not to wait for a championship if
 * a championship is joined 
 */
void CLIChampionshipView::onJoinedChampionship(){
	_waitForChamps = false;
}

/**
 * Method starting a match in a championship
 */
void CLIChampionshipView::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}