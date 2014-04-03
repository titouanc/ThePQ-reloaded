#include "CLITeamManager.hpp"
#include <match/CLIMatchManager.hpp>

using namespace SQUAD;

/// Constructor
CLITeamManager::CLITeamManager(ClientManager const & parent) : TeamManager(parent)
{}

/**
 * Method handling the team management interface
 */
void CLITeamManager::run()
{
	Menu _menu;
	_menu.addToDisplay("	- Put player at the selected position");
	_menu.addToDisplay("	- Swap player at the selected position with another player");
	_menu.addToDisplay("	- Upgrade player abilities");
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
			case 3:
				showPlayerAttributes();
				break;
			default:
				cout << "\033[2J\033[1;1H";			
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
	CLIMatchManager match(*this); 
	match.run();
}

/**
 * Method upgrading a players ability
 * @param int : id of the players whose ability should be upgraded
 */
void CLITeamManager::handleAbility(int id){
	cout << "   \033[36m 1     	    2            3      	   4" 
		<< "		<<<id of the ability to modify\033[0m"<< endl;
	cout << "Strength	Velocity	Precision	Chance		ID	 Name"<<endl;
	showPlayer(user().players[id-1],id);
	cout << "Enter the ability you wish to upgrade: ";
	int idAbility;
	Player &player = user().players[id-1];
	int strength = player.getStrength();
	int velocity = player.getVelocity();
	int precision = player.getPrecision();
	int chance = player.getChance();
	for(;;){
		if (cin >> idAbility){
			if(idAbility < 1 || idAbility > 4 ){
				cout << "Wrong option entered. Try again: ";
				cin.clear();
			}else{
				switch(idAbility){
					case 1:{
						if (strength+1 < 100 && strength+1 < user().acPoints){
							player.improveStrength(1);
							upgradePlayerAbility(player.getMemberID(),0);
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 2:{
						if (velocity+1 < 100 && velocity+1 < user().acPoints){
							player.improveVelocity(1);
							upgradePlayerAbility(player.getMemberID(),1);
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 3:{
						if (precision+1 < 100 && precision+1 < user().acPoints){
							player.improvePrecision(1);
							upgradePlayerAbility(player.getMemberID(),2);
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 4:{
						if (chance+1 < 100 && chance+1 < user().acPoints){
							player.improveChance(1);
							upgradePlayerAbility(player.getMemberID(),3);
						}else{
							showPlayerAttributes();
						}
						break;	
					}
					default:break;
				}
				onPlayersLoad();
				showPlayers();
				onTeamInfoChange();
				showPlayerAttributes();
			}

		}else{
			cout << "Wrong option entered. Try again: ";
			cin.clear();
		}
	}
	
}

void CLITeamManager::handlePlayerAbility(){
	cout << endl << endl <<  "Enter the id of the player whose ability to Upgrade: ";
	unsigned int idPlayer;
	for(;;){
		if (cin >> idPlayer){
			if(idPlayer<1 || idPlayer > user().players.size()){
				cout << "Wrong option entered. Try again: ";
				cin.clear();
			}else {
				cout << "\033[2J\033[1;1H";
				handleAbility(idPlayer);
			}
		}else{
			cout << "Wrong option entered. Try again: ";
			cin.clear();
		}
	}
}

void CLITeamManager::showPlayer(Player &player, int i){
	cout << showStrength(player.getStrength()) << "	" << showVelocity(player.getVelocity()) << "	" << showPrecision(player.getPrecision()) 
			<< "	" << showChance(player.getChance()) << "	" << i << "	 " <<player.getName()<<endl;
}
void CLITeamManager::showPlayerAttributes(){
	cout << "\033[2J\033[1;1H";
	cout << "========== Your players and their attributes =========="<<endl;
	cout << "************* Your Activity points: " << user().acPoints << " *****************" << endl;
	cout << endl << "Strength	"<<"Velocity	"<< "Precision	"<<"Chance		"<<"ID	 "<<"Name"<<endl;
	int i(1);
	for(Player &player : user().players){
		showPlayer(player,i);
		i++;
	}
	
	Menu _menu;
	_menu.addToDisplay(" - Upgrade a players ability ");
	_menu.addToDisplay(" - Return to the previous menu");
	int option = _menu.run();
	switch(option){
		case 1 :
			handlePlayerAbility();
			break;
		case 2 :
			cout << "\033[2J\033[1;1H";	
			break; 
		default : 
			cout << "\033[2J\033[1;1H";	
			break;
	}
}

std::string CLITeamManager::paddValue(int value){
	if (value < 10)
		return std::to_string(value)+"  ";
	else if (value<100)
		return std::to_string(value)+" ";
	else return std::to_string(value);
}

std::string CLITeamManager::showStrength(int value){
	std::string toShow;
	toShow+=paddValue(value);
	if(value < 100 && value+1 <= user().acPoints){
		toShow+="  ";
		toShow+="\033[33m";
		toShow+=paddValue(value+1);
		toShow+="\033[0m";
		toShow+="  ";
		return toShow;
	}else{
		toShow+="	";
	}
	return toShow;
}

std::string CLITeamManager::showPrecision(int value){
	std::string toShow;
	toShow+=paddValue(value);
	if(value < 100 && value+1 <= user().acPoints){
		toShow+="  ";
		toShow+="\033[33m";
		toShow+=paddValue(value+1);
		toShow+="\033[0m";
		toShow+="  ";
		return toShow;
	}else{
		toShow+="	";
	}
	return toShow;
}

std::string CLITeamManager::showChance(int value){
	std::string toShow;
	toShow+=paddValue(value);
	if(value < 100 && value+1 <= user().acPoints){
		toShow+="  ";
		toShow+="\033[33m";
		toShow+=paddValue(value+1);
		toShow+="\033[0m";
		toShow+="  ";
		return toShow;
	}else{
		toShow+="	";
	}
	return toShow;
}

std::string CLITeamManager::showVelocity(int value){
	std::string toShow;
	toShow+=paddValue(value);
	if(value < 100 && value+1 <= user().acPoints){
		toShow+="  ";
		toShow+="\033[33m";
		toShow+=paddValue(value+1);
		toShow+="\033[0m";
		toShow+="  ";
		return toShow;
	}else{
		toShow+="	";
	}
	return toShow;
}




