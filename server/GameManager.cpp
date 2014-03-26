#include "GameManager.hpp"



void GameManager::sendConnectedUsersList(int peer_id)
{
	JSON::List list;
	std::string userFrom = _users.find(peer_id)->second->getUsername();
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++)
	{
		if (it->second->getUsername() != userFrom)
		{
			list.append(it->second->getUsername());
		}
	}

	JSON::Dict usersList;
	usersList.set("type", MSG::CONNECTED_USERS_LIST);
	usersList.set("data", list);

	_outbox.push(Message(peer_id, usersList.clone()));
}

void GameManager::sendInvitationToPlayer(string const& userTo, int peer_id){
	map<int, User*>::iterator findUser = _users.find(peer_id);
	std::string userFrom = findUser->second->getUsername();
	cout << "from: " << userFrom;
	cout << "to: " << userTo;
	map<int, User*>::iterator it = _users.begin();
	bool found = false;
	if (userFrom != userTo)
	{
		while (it!=_users.end()){
			if (it->second->getUsername() == userTo){
				found = true;
				JSON::Dict toSend;
				toSend.set("type", MSG::FRIENDLY_GAME_INVITATION);
				toSend.set("data", userFrom);
				Message status(it->first, toSend.clone());
				_outbox.push(status);
			}
			it++;
		}
	}
	if (userFrom == userTo || !found)
	{
		JSON::Dict toSend, data;
		data.set("username", userTo);
		data.set("answer", MSG::USER_NOT_FOUND);
		toSend.set("type", MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
		toSend.set("data", data);
		Message status(peer_id, toSend.clone());
		_outbox.push(status);
	}
}

void GameManager::sendInvitationResponseToPlayer(const JSON::Dict &response, int peer_id){
	string username;
	if (ISSTR(response.get("username"))) 
		username = STR(response.get("username")).value();
	string answer;
	if (ISSTR(response.get("answer")))
		answer = STR(response.get("answer")).value();
	map<int, User*>::iterator findSender = _users.find(peer_id);
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++){
		if (it->second->getUsername() == username){
			JSON::Dict toSend;
			toSend.set("type", MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
			JSON::Dict data;
			data.set("username", findSender->second->getUsername());
			data.set("answer", answer);
			toSend.set("data", data);
			Message status(it->first, toSend.clone());
			_outbox.push(status);
			if (answer == MSG::FRIENDLY_GAME_INVITATION_ACCEPT){
				startMatch(peer_id, it->first, false);
			}
		}
	}
}

void GameManager::startMatch(int client_idA, int client_idB, bool champMatch)
{
	if (_users.find(client_idA) == _users.end() || 
		_users.find(client_idB) == _users.end()){
		return;
	}

	Squad & squad1 = _users[client_idA]->getTeam().getSquad();
	squad1.client_id = client_idA;

	Squad & squad2 = _users[client_idB]->getTeam().getSquad();
	squad2.client_id = client_idB;

	while (_outbox.available()); /* Clear outbox (do not lose msgs) */
	MatchManager *match = new MatchManager(_connectionManager, squad1, squad2, champMatch);
	match->start();
	_matches.push_back(match);
}