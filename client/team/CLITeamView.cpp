#include "CLITeamView.hpp"
#include <match/CLIMatchView.hpp>
#include <user/CLIUserView.hpp>
using namespace SQUAD;

/// Constructor
CLITeamView::CLITeamView(ClientController const & parent) : TeamController(parent)
{}
std::string CLITeamView::paddName(std::string name){
    std::string newName="";
    if (name.length()>35) return name;
    else{
        newName+=name;
        for(unsigned int i(0);i<35-name.length();i++){
            newName+=" ";
        }
        return newName;
    }
}
/**
 * Method handling the team management interface
 */
void CLITeamView::run()
{
	Menu _menu;
	_menu.addToDisplay("	- Put player at the selected position");
	_menu.addToDisplay("	- Swap player at the selected position with another player");
	_menu.addToDisplay("	- Upgrade player abilities");
	_menu.addToDisplay("	- Quit to management menu\n");
	int option;
	_wait = false;
	do
	{
		clScreen();
		do {
			minisleep(0.1);
			readMessages();
		}
		while (_wait == true);
		showPlayers();
		option = _menu.run();
		switch(option)
		{
			case 1:
				showSelectPlayer();
				break;
			case 2:
				showSwapPlayer();
				break;
			case 3:
				showPlayerAttributes();
				break;
			default:
				break;
		}
	}
	while (option != 4);
}

/**
 * Method displaying the teams players
 */
void CLITeamView::showPlayers(){
	cout << "================================================ YOUR PLAYERS =======================================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl; //modif
	}
	cout << "=====================================================================================================" << endl;
}

/**
 * Method displaying the users squad
 */
void CLITeamView::showSquad()
{
	cout << "================================================= YOUR SQUAD ========================================" << endl;
	cout << LEFT_CHASER << " - \033[34mLeft Chaser\033[0m : 		"
		 << paddName(user().squad.players[LEFT_CHASER]->getName()) << " - ID: " << user().squad.players[LEFT_CHASER]->getMemberID() << endl;
	cout << CENTER_CHASER << " - \033[34mCenter Chaser\033[0m : 		"
	 	 << paddName(user().squad.players[CENTER_CHASER]->getName()) << " - ID: " << user().squad.players[CENTER_CHASER]->getMemberID() << endl;
	cout << RIGHT_CHASER << " - \033[34mRight Chaser\033[0m : 		"
	     << paddName(user().squad.players[RIGHT_CHASER]->getName()) << " - ID: " << user().squad.players[RIGHT_CHASER]->getMemberID() << endl;
	cout << LEFT_BEATER << " - \033[34mLeft Beater\033[0m : 		"
	     << paddName(user().squad.players[LEFT_BEATER]->getName()) << " - ID: " << user().squad.players[LEFT_BEATER]->getMemberID() << endl;
	cout << RIGHT_BEATER << " - \033[34mRight Beater\033[0m : 		"
	     << paddName(user().squad.players[RIGHT_BEATER]->getName()) << " - ID: " << user().squad.players[RIGHT_BEATER]->getMemberID() << endl;
	cout << SEEKER << " - \033[34mSeeker\033[0m : 			"
	     << paddName(user().squad.players[SEEKER]->getName()) << " - ID: " << user().squad.players[SEEKER]->getMemberID() << endl;
	cout << KEEPER << " - \033[34mKeeper\033[0m : 			"
	     << paddName(user().squad.players[KEEPER]->getName()) << " - ID: " << user().squad.players[KEEPER]->getMemberID() << endl;
	cout << "=====================================================================================================" << endl;
}

/**
 * Method handling user input for a players position
 */
void CLITeamView::showSelectPosition()
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
void CLITeamView::showSelectPlayer()
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
void CLITeamView::showSwapPlayer()
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
void CLITeamView::onMatchStart(){
	CLIMatchView match(*this); 
	match.run();
}

/**
 * Method upgrading a players ability
 * @param int : id of the players whose ability should be upgraded
 */
void CLITeamView::handleAbility(int id){
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
	bool c = true;
	do {
		if (cin >> idAbility){
			if(idAbility < 1 || idAbility > 4 ){
				cout << "Wrong option entered. Try again: ";
				c = false;
				cin.clear();
				cin.ignore(100,'\n');
			}else{
				c=true;
				switch(idAbility){

					case 1:{
						if (strength+1 < 100 && strength+1 < user().acPoints){
							player.improveStrength(1);
							upgradePlayerAbility(player.getMemberID(),0);
							onTeamInfoChange();
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 2:{
						if (velocity+1 < 100 && velocity+1 < user().acPoints){
							player.improveVelocity(1);
							upgradePlayerAbility(player.getMemberID(),1);
							onTeamInfoChange();
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 3:{
						if (precision+1 < 100 && precision+1 < user().acPoints){
							player.improvePrecision(1);
							upgradePlayerAbility(player.getMemberID(),2);
							onTeamInfoChange();
						}else{
							showPlayerAttributes();
						}
						break;
					}
					case 4:{
						if (chance+1 < 100 && chance+1 < user().acPoints){
							player.improveChance(1);
							upgradePlayerAbility(player.getMemberID(),3);
							onTeamInfoChange();
						}else{
							showPlayerAttributes();
						}
						break;	
					}
					default:break;
				}
				
				onPlayersLoad();
				onTeamInfoChange();
				run();
			}
		}else{
			c=false;
			cout << "Wrong option entered. Try again: ";
			cin.clear();
			cin.ignore(100,'\n');
		}
	}while (!c);
	
}

/**
 * Method handling the player ability upgrade interface
 */
void CLITeamView::handlePlayerAbility(){
	cout << endl << endl <<  "Enter the id of the player whose ability to Upgrade: ";
	unsigned int idPlayer;
	for(;;){
		if (cin >> idPlayer){
			if(idPlayer<1 || idPlayer > user().players.size()){
				cout << "Wrong option entered. Try again: ";
				cin.clear();
			}else {
				clScreen();
				handleAbility(idPlayer);
			}
		}else{
			cout << "Wrong option entered. Try again: ";
			cin.clear();
		}
	}
}

/**
 * Method displaying the player abilities
 */
void CLITeamView::showPlayer(Player &player, int i){
	cout << showStrength(player.getStrength()) << "	" << showVelocity(player.getVelocity()) << "	" << showPrecision(player.getPrecision()) 
			<< "	" << showChance(player.getChance()) << "	" << i << "	 " <<player.getName()<<endl;
}

/**
 * Method showing the abilities of the whole squad
 */
void CLITeamView::showPlayerAttributes(){
	clScreen();
	onTeamInfoChange();
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
			clScreen();
			run();
			break;
		default : 
			clScreen();	
			break;
	}
}

/**
 * Method padding a value for 3 decimals
 * @param value to be padded
 */
std::string CLITeamView::paddValue(int value){
	if (value < 10)
		return std::to_string(value)+"  ";
	else if (value<100)
		return std::to_string(value)+" ";
	else return std::to_string(value);
}

/**
 * Method displaying the strength level and the upgraded level
 */
std::string CLITeamView::showStrength(int value){
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

/**
 * Method displaying the precision level and the upgraded level
 */
std::string CLITeamView::showPrecision(int value){
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

/**
 * Method displaying the chance level and the upgraded level
 */
std::string CLITeamView::showChance(int value){
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

/**
 * Method displaying the velocity level and the upgraded lelvel
 */
std::string CLITeamView::showVelocity(int value){
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




