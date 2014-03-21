#ifndef __GAME_MANAGER_HPP
#define __GAME_MANAGER_HPP

#include "ServerManager.hpp"

class GameManager : public ServerManager
{
public:
	using ServerManager::ServerManager;
	GameManager(const ServerManager& parent) : ServerManager(parent){}
	
	void sendConnectedUsersList(int peer_id);
    void sendInvitationToPlayer(string const& username, int peer_id);
    void sendInvitationResponseToPlayer(const JSON::Dict &response, int peer_id);
    void startMatch(int client_idA, int client_idB, bool champMatch);
};

#endif // __GAME_MANAGER_HPP