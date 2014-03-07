#include "Client.hpp"
#include <pthread.h>
#include "CLIMatchManager.hpp"

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Client::Client(NetConfig const &config) : 	_user(), _notifications(), _connection(config.host, config.port),
												_userManager(_connection, _user, _notifications),
												_matchManager(_connection, _user),
												_isRunning(true)
{}

Client::~Client()
{}

void Client::run()
{
	_connection.run();
	_userManager.run();
}

/* Friendly match menu */
void Client::showFriendlyMatchMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - list all connected players\n");
	_menu.addToDisplay("   - choose one to play a friendly game with\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				printConnectedUsersList();
				break;
			case 2:
				showChooseUserMenu();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

void Client::showChooseUserMenu()
{
	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
	_matchManager.chooseUser(userInput);
	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
	try {
		_matchManager.waitForUser();
		cout << userInput << " accepted your invitation!" << endl;
		_matchManager.startMatch();
		showTurnMenu();
	}
	catch (UserNotFoundException &e)
	{
		cout << "Username not found" << endl;
	}
	catch (UserDeniedException &e)
	{
		cout << userInput << " denied your invitation. Sorry!" << endl;		
	}
}

void Client::printConnectedUsersList(){
	vector<std::string> connectedUsers = _matchManager.getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
}

void Client::handleFriendlyGameInvitation(JSON::Value const *json){
	if (ISSTR(json)){
		string user = STR(json).value();
		cout << user << " invited you to a game" << endl;
		// bool handle = askForNotificationHandling();
		bool handle = true;
		Menu _menu;
		_menu.addToDisplay("   - accept\n");
		_menu.addToDisplay("   - deny\n");
		int option;
		while(handle)
		{
			option = _menu.run();
			if (option == 1){
				handle = false;
				
				//_matchManager.acceptInvitationFromUser(user);
				//showTurnMenu();
				// TODO start to play game
				CLIMatchManager YAYAYA(_connection, _user, _notifications);
				YAYAYA.run();
				break;
			} else if (option == 2){
				handle = false;
				_matchManager.denyInvitationFromUser(user);
				break;
			} else {
				cout << "Wrong option entered" << endl;
				break;
			}
		}
	}
}
// Match

void Client::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << playerLetter(*(_matchManager.getOwnSquad().players[i])) << " ";
		cout << _matchManager.getOwnSquad().players[i]->getName() << endl;
	}
}

Position Client::parseDirection(string userInput){
	Position res(0,0);
	if (userInput.size() == 1){
		if (userInput == "e")
			res = Pitch::East;
		else if (userInput == "w")
			res = Pitch::West;
	}
	else if (userInput.size() == 2){
		if (userInput == "nw")
			res = Pitch::NorthWest;
		else if (userInput == "ne")
			res = Pitch::NorthEast;
		else if (userInput == "sw")
			res = Pitch::SouthWest;
		else if (userInput == "se")
			res = Pitch::SouthEast;
	}
	return res;
}

char Client::playerLetter(Player const & player)
{
	return 'A' + player.getID() - 1;
}

std::string Client::colorPlayerLetter(Player const & player)
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
	res[5] = playerLetter(player);
	return std::string(res);
}

void Client::displayPitch()
{
	Moveable *atPos = NULL;
	Pitch _pitch = _matchManager.getPitch();
	for (int y=_pitch.ymax()-1; y>=_pitch.ymin(); y--){
		for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){

			/* background */
			if (_pitch.isInKeeperZone(x, y))
				cout << "\033[47m";
			else if (_pitch.inEllipsis(x, y))
				cout << "\033[42m";

			/* foreground */
			if (! _pitch.isValid(x, y) || ! _pitch.inEllipsis(x, y)){
				cout << ' ';
			} else {
				atPos = _pitch.getAt(x, y);
				if (_pitch.isGoal(x, y)){
					cout << "\033[1m\033[44mO";
				} else if (atPos==NULL){
					cout << '.';
				} else if (atPos->isBall()){
					/* Colorize balls by type */
					Ball const & ball = (Ball const &) *atPos;
					cout << "\033[1m";
					if (ball.isGoldenSnitch())
						cout << "\033[33m";
					else if (ball.isQuaffle())
						cout << "\033[34m";
					else if (ball.isBludger())
						cout << "\033[31m";
					cout << '*';
				} else if (atPos->isPlayer()){
					/* Colorize players by type */
					Player const & player = (Player const &) *atPos;
					
					if (_matchManager.isOwnPlayer(player))
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
	     << "\033[34m*Quaffle \033[31m*Bludger \033[33m*Golden Snitch\033[0m"
		 << endl;
}


void Client::showTurnMenu(){
	
	Menu showTurnMenu;
	showTurnMenu.addToDisplay("\t- select player");
	int option;
	do {
		_matchManager.updatePitchWithDeltas();
		displayPitch();
		displayAvailablePlayers();
		option = showTurnMenu.run();
		switch(option){
			case 1:
				selectPlayer();
				break;
			default:
				break;
		}
		_connection.updateNotifications();
		_matchManager.updatePitchWithDeltas();
		displayPitch();
	} while (!_matchManager.isMatchFinished());
}

void Client::selectPlayer(){
	Menu selectPlayer;
	for (int i=0; i<7; ++i){
		selectPlayer.addToDisplay(
			std::string("\t- \033[1m") + 
			colorPlayerLetter(*(_matchManager.getOwnSquad().players[i])) +
			" - " +
			_matchManager.getOwnSquad().players[i]->getName() +
			"\033[0m"
		);
	}
	selectPlayer.addToDisplay("\t- back");
	int option = 0;
	if ((option = selectPlayer.run()) != 8){
		selectDirectionForPlayer(option);
	}
}

void Client::selectDirectionForPlayer(int player){
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
	_matchManager.sendStroke(player, currentDisplacement);
}
