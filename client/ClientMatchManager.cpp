#include "ClientMatchManager.hpp"
#include "Menu.hpp"

using namespace std;
using namespace net;

ClientMatchManager::ClientMatchManager() : _isMatchFinished(false) {}

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
	turnMenu();
}

void ClientMatchManager::startMatch(){
	cout << "Match Started! Have fuuuuuuuuuunnnnnn :D" << endl;
	cout << _pitch << endl;
	
}

void ClientMatchManager::selectPlayer(){

}

void ClientMatchManager::turnMenu(){
	startMatch();
	displayAvailablePlayers();
	Menu turnMenu;
	turnMenu.addOption("  - select player");
	int option;
	do {
		option = turnMenu.run();
		switch(option){
			default:
				break;
		}
	} while (!_isMatchFinished);

	
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





