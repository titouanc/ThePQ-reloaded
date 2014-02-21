#include "ClientMatchManager.hpp"
#include "Menu.hpp"

using namespace std;
using namespace net;

ClientMatchManager::ClientMatchManager(net::TcpSocket &connection) : _connection(connection),
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

void ClientMatchManager::initSquads(const JSON::Dict& msg, string username){
	if (ISLIST(msg.get("data")) && LIST(msg.get("data")).len() == 2) {
		JSON::List & squads = LIST(msg.get("data"));
		if (ISSTR(DICT(squads[0]).get("squad_owner")) && STR(DICT(squads[0]).get("squad_owner")).value() == username){
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
	int option;
	do {
		cout << _pitch << endl;
		displayAvailablePlayers();
		option = turnMenu.run();
		switch(option){
			case 1:
				selectPlayer();
			default:
				break;
		}
	} while (!_isMatchFinished);
}

void ClientMatchManager::selectPlayer(){
	Menu selectPlayer;
	for (int i=0; i<7; ++i){
		selectPlayer.addToDisplay("\t- " + _ownSquad.players[i]->getName());
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
	toSend.set("type", net::MSG::MATCH_STROKE);
	JSON::Dict data;
	data.set("mid", _ownSquad.players[player-1]->getID());
	data.set("move", currentDisplacement.toJson());
	toSend.set("data", data);
	_connection.send(toSend);
}

void ClientMatchManager::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << _ownSquad.players[i]->getID() << " ";
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





