#include "ClientMatchManager.hpp"

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
	data.set("move", (JSON::List)currentDisplacement);
	toSend.set("data", data);
	_connection.send(toSend);
}

void ClientMatchManager::updatePitchWithDeltas(){
	JSON::Value* msg = _connection.getNotification(MSG::MATCH_DELTA);
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

vector<std::string> ClientMatchManager::getConnectedUsersList(){// TODO
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::CONNECTED_USERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISLIST(received.get("data"))) {
		JSON::List & connectedUsers = LIST(received.get("data"));
		for (size_t i = 0; i<connectedUsers.len(); ++i)
			res.push_back(STR(connectedUsers[i]));
	}
	return res;
}

void ClientMatchManager::acceptInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT);
	toSend.set("data", data);
	_connection.send(toSend);
	startMatch();
}

void ClientMatchManager::denyInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_DENY);
	toSend.set("data", data);
	_connection.send(toSend);
}

void ClientMatchManager::startMatch(){
	JSON::Value *serverBalls = _connection.waitForMsg(net::MSG::MATCH_BALLS);
	JSON::Dict const &receivedBalls = DICT(serverBalls);
	initBalls(receivedBalls);
	JSON::Value *serverSquads = _connection.waitForMsg(net::MSG::MATCH_SQUADS);
	JSON::Dict const &receivedSquads = DICT(serverSquads);
	initSquads(receivedSquads);
	//_connection.waitForMsg(net::MSG::MATCH_START);
}


void ClientMatchManager::chooseUser(std::string userInput)
{
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_USERNAME);
	toSend.set("data", userInput);
	_connection.send(toSend);
}
void ClientMatchManager::waitForUser()
{
	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict const & received = DICT(serverMessage);
	if (ISDICT(received.get("data")) && ISSTR(DICT(received.get("data")).get("answer"))){
		string answer = STR(DICT(received.get("data")).get("answer")).value();
		if (answer == net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT){
			// Accepts
		}
		else if (answer == net::MSG::USER_NOT_FOUND) {
			throw UserNotFoundException();
		}
		else {
			throw UserDeniedException();
		}
	}
}

