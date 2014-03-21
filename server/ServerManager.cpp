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