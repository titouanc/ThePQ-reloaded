#include "ClientMatchManager.hpp"
#include "Menu.hpp"

using namespace std;
using namespace net;

ClientMatchManager::ClientMatchManager(SharedQueue<net::Message>&outbox, string username) : 	
				_outbox(outbox), 
				_isMatchFinished(false),
				_username(username){}

void ClientMatchManager::initSquads(const JSON::Value* msg){
	if (ISLIST(msg) && LIST(msg).len() == 2) {
		JSON::List & squads = LIST(msg);
		if (ISSTR(DICT(squads[0]).get("squad_owner")) && STR(DICT(squads[0]).get("squad_owner")).value() == _username){
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

void ClientMatchManager::startMatch(const JSON::Value* msg){
	cout << "Match Started! Have fuuuuuuuuuunnnnnn :D" << endl;
	cout << _pitch << endl;
	
}

void ClientMatchManager::selectPlayer(){

}

void ClientMatchManager::turnMenu(){
	displayAvailablePlayers();
	Menu turnMenu;
	turnMenu.addOption('s', new ClassCallback<ClientMatchManager>(this,&ClientMatchManager::selectPlayer), "(s)elect player");
	turnMenu.run();
}

void ClientMatchManager::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << _ownSquad.players[i]->getName() << " ";
		cout << _ownSquad.players[i]->getID() << endl;
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





