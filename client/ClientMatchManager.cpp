#include "ClientMatchManager.hpp"
#include "Menu.hpp"

using namespace std;
using namespace net;

ClientMatchManager::ClientMatchManager(ClientConnectionManager &connection, User &user) : 
																	_connection(connection),
																	_user(user),
																	 _isMatchFinished(false){}

void ClientMatchManager::initBalls(const JSON::Dict& msg){
	if (ISLIST(msg.get("data")) && LIST(msg.get("data")).len() == 4){
		JSON::List & balls = LIST(msg.get("data"));
		// TODO test ISDICT for each of the 4 balls
		_quaffle = DICT(balls[0]);
		_pitch.insert(&_quaffle);
		_snitch = DICT(balls[1]);
		_pitch.insert(&_snitch);
		for(int i=0; i<2; ++i){
			_bludgers[i] = DICT(balls[i+2]);
			_pitch.insert(&_bludgers[i]);
		}
	}
}

void ClientMatchManager::initSquads(const JSON::Dict& msg){
	if (ISLIST(msg.get("data")) && LIST(msg.get("data")).len() == 2) {
		JSON::List & squads = LIST(msg.get("data"));
		if (ISSTR(DICT(squads[0]).get("squad_owner")) && STR(DICT(squads[0]).get("squad_owner")).value() == _user.username){
			_ownSquad = Squad(DICT(squads[0]));
			_otherSquad = Squad(DICT(squads[1]));
		}
		else{
			_ownSquad = Squad(DICT(squads[1]));
			_otherSquad = Squad(DICT(squads[0]));
		}
		for (int i=0; i<7; ++i){
			_pitch.insert(_ownSquad.players[i]);
			_pitch.insert(_otherSquad.players[i]);
		}
	}
}

void ClientMatchManager::startMatch(){
	cout << "Match Started! Have fuuuuuuuuuunnnnnn :D" << endl;
	turnMenu();
}


void ClientMatchManager::turnMenu(){
	
	Menu turnMenu;
	turnMenu.addToDisplay("\t- select player");
	turnMenu.addToDisplay("\t- update pitch");
	int option;
	do {
		JSON::Value* deltas = _connection.hasMessageTypeInNotifications(MSG::MATCH_DELTA);
		if (ISDICT(deltas))
			updatePitchWithDeltas(DICT(deltas));
		displayPitch();
		displayAvailablePlayers();
		option = turnMenu.run();
		switch(option){
			case 1:
				selectPlayer();
			case 2:
				updatePitch();
			default:
				break;
		}
	} while (!_isMatchFinished);
}

void ClientMatchManager::updatePitch(){
	_connection.updateNotifications();
	cout << "75" << endl;
	JSON::Value* deltas = _connection.hasMessageTypeInNotifications(net::MSG::MATCH_DELTA);
	if (ISDICT(deltas)){
		cout << "78" << endl;
		updatePitchWithDeltas(DICT(deltas));
	}
	displayPitch();
}

void ClientMatchManager::selectPlayer(){
	Menu selectPlayer;
	for (int i=0; i<7; ++i){
		selectPlayer.addToDisplay(
			std::string("\t- \033[1m") + 
			colorPlayerLetter(*(_ownSquad.players[i])) +
			" - " +
			_ownSquad.players[i]->getName() +
			"\033[0m"
		);
	}
	selectPlayer.addToDisplay("\t- back");
	int option = 0;
	if ((option = selectPlayer.run()) != 8){
		selectDirectionForPlayer(option);
	}
}

void ClientMatchManager::selectDirectionForPlayer(int player){
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
	JSON::Dict toSend;
	toSend.set("type", MSG::MATCH_STROKE);
	JSON::Dict data;
	data.set("mid", _ownSquad.players[player-1]->getID());
	data.set("move", currentDisplacement.toJson());
	toSend.set("data", data);
	_connection.send(toSend);

	updatePitch();
}

void ClientMatchManager::updatePitchWithDeltas(JSON::Dict& deltas){
	cout << "148" << endl;
	if (ISLIST(deltas.get("data"))){
		JSON::List const & toMove = LIST(deltas.get("data"));
		for(size_t i = 0; i<toMove.len(); ++i){
			if (ISDICT(toMove[i])){
				cout << "153" << endl;
				JSON::Dict const & movement = DICT(toMove[i]);
				if(ISLIST(movement.get("from")) && ISLIST(movement.get("to"))){
					Position fromPos(LIST(movement.get("from")));
					Position toPos(LIST(movement.get("to")));
					Moveable *atPos = _pitch.getAt(fromPos);
					if (atPos != NULL){
						_pitch.setAt(toPos, atPos);
						_pitch.setAt(fromPos, NULL);
					}
					cout << "Moving " << fromPos.toJson() << " -> " << toPos.toJson() << endl;
				}
			}
		}
	}
}

void ClientMatchManager::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << playerLetter(*(_ownSquad.players[i])) << " ";
		cout << _ownSquad.players[i]->getName() << endl;
	}
}

Position ClientMatchManager::parseDirection(string userInput){
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

bool ClientMatchManager::isOwnPlayer(Player const & player)
{
	for (int i=0; i<7; i++)
		if (_ownSquad.players[i] == &player)
			return true;
	return false;
}

char ClientMatchManager::playerLetter(Player const & player)
{
	return 'A' + player.getID() - 1;
}

std::string ClientMatchManager::colorPlayerLetter(Player const & player)
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

void ClientMatchManager::displayPitch()
{
	Moveable *atPos = NULL;
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
					
					if (isOwnPlayer(player))
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




