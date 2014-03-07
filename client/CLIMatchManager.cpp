#include "CLIMatchManager.hpp"

void CLIMatchManager::selectPlayer()
{
	displayAvailablePlayers();
	cout << "Choose a player to move: " << endl;
	Menu menu;
	for (int i=0; i<7; i++){
		Player const & p = *(mySquad().players[i]);
		menu.addToDisplay(
			std::string("\033[1m")+
			colorPlayerLetter(p)+" "+
			p.getName()+"\033[0m"
		);
	}
	menu.addToDisplay("Go back");
	int choice = menu.run();
	selectDirectionForPlayer(choice);
}

void CLIMatchManager::run()
{
	cout << "CLIMatchManager started" << endl;
	while (state() != FINISHED){
		readMessages();
		displayPitch();
		selectPlayer();
	}
	cout << "CLIMatchManager ended" << endl;
}

void CLIMatchManager::onStateChange()
{
	if (state() == PROMPT)
		cout << "You can play now !" << endl;
	else if (state() == TIMEOUT)
		cout << "Your play time is finished." << endl;
}

void CLIMatchManager::onPitchChange()
{
	displayPitch();
}

/* Return colored letter for a player (without cleanup escape code) */
std::string CLIMatchManager::colorPlayerLetter(Player const & player)
{
	char res[7] = "\033[30mX";
	if (player.isSeeker())
		res[3] = '3';
	else if (player.isChaser())
		res[3] = '4';
	else if (player.isBeater())
		res[3] = '1';
	else if (player.isKeeper())
		res[3] = '6';
	res[5] = 'A' + player.getID() - 1;
	return isMyPlayer(player) ? std::string("\033[1m")+res : std::string(res);
}

/* Return a colored star for a ball */
std::string CLIMatchManager::colorBallLetter(Ball const & ball)
{
	char res[7] = "\033[30m*";
	if (ball.isGoldenSnitch())
		res[3] = '3';
	else if (ball.isQuaffle())
		res[3] = '4';
	else if (ball.isBludger())
		res[3] = '1';
	return std::string(res);
}

void CLIMatchManager::displayPitch()
{
	Moveable *atPos = NULL;
	for (int y=pitch().ymax()-1; y>=pitch().ymin(); y--){
		for (int x=pitch().xmin(); x<pitch().xmax(); x++){

			/* background */
			if (pitch().isInKeeperZone(x, y))
				cout << "\033[47m";
			else if (pitch().inEllipsis(x, y))
				cout << "\033[42m";

			/* foreground */
			if (! pitch().isValid(x, y) || ! pitch().inEllipsis(x, y)){
				cout << ' ';
			} else {
				atPos = pitch().getAt(x, y);
				if (pitch().isGoal(x, y)){
					cout << "\033[1m\033[44mO";
				} else if (atPos==NULL){
					cout << '.';
				} else if (atPos->isBall()){
					/* Colorize balls by type */
					Ball const & ball = (Ball const &) *atPos;
					cout << colorBallLetter(ball);
				} else if (atPos->isPlayer()){
					/* Colorize players by type */
					Player const & player = (Player const &) *atPos;
					
					if (isMyPlayer(player))
						cout << "\033[1m";
					cout << colorPlayerLetter(player);
				}
			}
			cout << "\033[0m";
		}
		cout << endl;
	}
	cout << "\033[1m"
		 << "\033[34mChaser \033[31mBeater \033[36mKeeper \033[33mSeeker"
		 << "\033[0m :: \033[1m"
	     << "\033[34m*Quaffle \033[31m*Bludger \033[33m*Golden Snitch\033[0m"
		 << endl;
}

// Position CLIMatchManager::parseDirection(string userInput){
// 	Position res(0,0);
// 	if (userInput.size() == 1){
// 		if (userInput == "e")
// 			res = Pitch::East;
// 		else if (userInput == "w")
// 			res = Pitch::West;
// 	}
// 	else if (userInput.size() == 2){
// 		if (userInput == "nw")
// 			res = Pitch::NorthWest;
// 		else if (userInput == "ne")
// 			res = Pitch::NorthEast;
// 		else if (userInput == "sw")
// 			res = Pitch::SouthWest;
// 		else if (userInput == "se")
// 			res = Pitch::SouthEast;
// 	}
// 	return res;
// }

char CLIMatchManager::playerLetter(Player const & player)
{
	return 'A' + player.getID() - 1;
}


void CLIMatchManager::selectDirectionForPlayer(int player){
	Displacement currentDisplacement;
	Menu selectDirection;
	selectDirection.addToDisplay("\t- NorthEast");
	selectDirection.addToDisplay("\t- East");
	selectDirection.addToDisplay("\t- SouthEast");
	selectDirection.addToDisplay("\t- SouthWest");
	selectDirection.addToDisplay("\t- West");
	selectDirection.addToDisplay("\t- NorthWest");
	selectDirection.addToDisplay("\t- done");
	int option = 0;
	while ((option = selectDirection.run()) != 7){
		switch (option){
			case 1:
				currentDisplacement.addMove(Pitch::NorthEast);
				break;
			case 2:
				currentDisplacement.addMove(Pitch::East);
				break;
			case 3:
				currentDisplacement.addMove(Pitch::SouthEast);
				break;
			case 4:
				currentDisplacement.addMove(Pitch::SouthWest);
				break;
			case 5:
				currentDisplacement.addMove(Pitch::West);
				break;
			case 6:
				currentDisplacement.addMove(Pitch::NorthWest);
				break;
			default:
				break;
		}
	}
	sendDisplacement(*(mySquad().players[player]), currentDisplacement);
}

void CLIMatchManager::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << playerLetter(*(mySquad().players[i])) << " ";
		cout << mySquad().players[i]->getName() << endl;
	}
}