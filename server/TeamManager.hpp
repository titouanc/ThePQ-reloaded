#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include "ServerManager.hpp"

class TeamManager : public ServerManager
{
public:
	using ServerManager::ServerManager;
	TeamManager(const ServerManager & parent) : ServerManager(parent){}

    void putPlayerOnSquadPosition(const JSON::Dict &response, int peer_id);
    void swapPlayersOfSquad(const JSON::Dict &response, int peer_id);
    void upgradePlayerAbility(const JSON::Dict &response, int peer_id);
};

#endif // __TEAM_MANAGER_HPP