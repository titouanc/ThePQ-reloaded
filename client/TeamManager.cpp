#include "TeamManager.hpp"

void TeamManager::loadPlayers(){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, _user._username);//modif
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::PLAYERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	JSON::List toFill;
	if(ISLIST(received.get("data"))){
		toFill = LIST(received.get("data"));
	}
	_user.players.clear();
	for(size_t i=0; i<toFill.len();++i){
		Player player(DICT(toFill[i]));
		myplayers.push_back(_user.players);
	}
	delete serverResponse;
}
