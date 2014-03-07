#include "CLIMatchManager.hpp"

void CLIMatchManager::selectPlayer()
{
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
	menu.addToDisplay("Refresh");
	int choice = menu.run();
	if (1 <= choice && choice <= 7){
		selectDirectionForPlayer(mySquad().players[choice-1]);
	}
}

void CLIMatchManager::run()
{
	cout << "CLIMatchManager started" << endl;
	while (state() != READY)
		readMessage();
	while (state() != FINISHED){
		readMessages();
		displayPitch();
		selectPlayer();
	}
	cout << "CLIMatchManager ended" << endl;
}

void CLIMatchManager::onStateChange()
{
	cout << "\t\033[35mMATCH STATE: ";
	switch (state()){
		case CREATED: cout << "CREATED"; break;
		case READY: cout << "READY"; break;
		case PROMPT: cout << "PROMPT"; break;
		case TIMEOUT: cout << "TIMEOUT"; break;
		case FINISHED: cout << "FINISHED"; break;
	}
	cout << "\033[0m" << endl;
}

void CLIMatchManager::onPitchChange()
{
	displayPitch();
}

void CLIMatchManager::onError(std::string const & info)
{
	cout << "\033[1m\033[31m" << info << "\033[0m" << endl;
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

void CLIMatchManager::selectDirectionForPlayer(Player *player)
{
	Displacement move;
	Menu selectDirection;

	/* create direction menu */
	for (int i=0; i<6; i++)
		selectDirection.addToDisplay(Pitch::directions_names[i]);
	selectDirection.addToDisplay("End move");

	/* prompt user */
	for (
		int option=selectDirection.run(); 
		move.length()<player->getSpeed(); 
		option=selectDirection.run()
	){
		if (option == 7)
			break;
		move.addMove(Pitch::directions[option-1]);
	}

	/* Send displacement to server */
	if (move.length() > 0)
		sendDisplacement(*player, move);
}
