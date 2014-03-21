#include "ServerManager.hpp"


User *ServerManager::getUserByName(std::string username)
{
	std::map<int, User*>::const_iterator iter;
	for (iter=_users.begin(); iter!=_users.end(); iter++)
		if (username == iter->second->getUsername())
			return iter->second;
	return NULL;
}


void ServerManager::sendTeamInfos(const JSON::Dict &data, int peer_id)
{
	JSON::Dict response;
	response.set("type", net::MSG::TEAM_INFOS);
	response.set("data", data);
	_outbox.push(Message(peer_id, response.clone()));
}

void ServerManager::sendPlayersList(int peer_id){
	std::vector<Player> & players = _users[peer_id]->getTeam().getPlayers();
	JSON::List jsonPlayers;
	for(size_t i = 0; i<players.size();++i){
		jsonPlayers.append(JSON::Dict(players[i]));
	}
	JSON::Dict toSend;
	toSend.set("type",net::MSG::PLAYERS_LIST);
	toSend.set("data", jsonPlayers);
	Message status(peer_id, toSend.clone());
	_outbox.push(status);
}


int ServerManager::getPeerID(std::string const &username){
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++){
		if (it->second->getUsername() == username){
			return it->first;
		}
	}
	return -1;
}