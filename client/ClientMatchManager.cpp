#include "ClientMatchManager.hpp"
#include "Menu.hpp"

using namespace std;
using namespace net;

ClientMatchManager::ClientMatchManager(ClientConnectionManager &connection, UserData &user) : 
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

void ClientMatchManager::sendStroke(int player, Displacement& currentDisplacement)
{
	JSON::Dict toSend;
	toSend.set("type", MSG::MATCH_STROKE);
	JSON::Dict data;
	data.set("mid", _ownSquad.players[player-1]->getID());
	data.set("move", currentDisplacement.toJson());
	toSend.set("data", data);
	_connection.send(toSend);
}

void ClientMatchManager::updatePitchWithDeltas(){
	JSON::Value* msg = _connection.hasMessageTypeInNotifications(MSG::MATCH_DELTA);
	if (ISDICT(msg))
	{
		JSON::Dict const & deltas = DICT(msg);
		if (ISLIST(deltas.get("data"))){
			JSON::List const & toMove = LIST(deltas.get("data"));
			for(size_t i = 0; i<toMove.len(); ++i){
				if (ISDICT(toMove[i])){
					JSON::Dict const & movement = DICT(toMove[i]);
					if(ISLIST(movement.get("from")) && ISLIST(movement.get("to"))){
						Position fromPos(LIST(movement.get("from")));
						Position toPos(LIST(movement.get("to")));
						Moveable *atPos = _pitch.getAt(fromPos);
						if (atPos != NULL){
							_pitch.setAt(toPos, atPos);
							_pitch.setAt(fromPos, NULL);
						}
						//~ cout << "Moving " << fromPos.toJson() << " -> " << toPos.toJson() << endl; // TODO WHAT ?
					}
				}
			}
		}
		delete msg;
	}
}

bool ClientMatchManager::isOwnPlayer(Player const & player)
{
	for (int i=0; i<7; i++)
		if (_ownSquad.players[i] == &player)
			return true;
	return false;
}




