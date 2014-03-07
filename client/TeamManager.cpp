#include "TeamManager.hpp"

TeamManager::TeamManager(ClientManager const & parent) : ClientManager(parent) 
{}

void TeamManager::loadPlayers(){
	user().players.clear();
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, user().username);
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	connection().send(query);
	
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::PLAYERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	JSON::List toFill;
	if(ISLIST(received.get("data"))){
		toFill = LIST(received.get("data"));
	}
	user().players.clear();
	for(size_t i=0; i<toFill.len();++i){
		Player player(DICT(toFill[i]));
		user().players.push_back(player);
	}
	delete serverResponse;
}
