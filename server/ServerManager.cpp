#include "ServerManager.hpp"


User *ServerManager::getUserByName(std::string username)
{
	std::map<int, User*>::const_iterator iter;
	for (iter=_users.begin(); iter!=_users.end(); iter++)
		if (username == iter->second->getUsername())
			return iter->second;
	return NULL;
}

/// Method sending the team attributes stored on the server to the user
void ServerManager::sendTeamInfos(const JSON::Dict &data, int peer_id)
{
	JSON::Dict response;
	response.set("type", net::MSG::TEAM_INFOS);
	response.set("data", data);
	_outbox.push(Message(peer_id, response.clone()));
}

/// Method sending the player attributes stored on the server to the user
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

/// Method retrieving the id of the user based on the username
int ServerManager::getPeerID(std::string const &username){
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++){
		if (it->second->getUsername() == username){
			return it->first;
		}
	}
	return -1;
}

/// Method sending notifications from the server to the user
void ServerManager::sendNotification(std::string username, const JSON::Dict & toSend){
	int to_peer = getPeerID(username);
	if (to_peer >= 0){
		/* User currently connected to server */
		Message status(to_peer, toSend.clone());
		_outbox.push(status); 
	} else {
		User *user = User::load(username);
		if (user != NULL){
			user->sendOfflineMsg(toSend);
			delete user;
		}
	}
}
