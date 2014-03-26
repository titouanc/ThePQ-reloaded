#ifndef __USER_MANAGER_HPP
#define __USER_MANAGER_HPP

#include "ServerManager.hpp"

class UserManager : public ServerManager
{
public:
	using ServerManager::ServerManager;
	UserManager(const ServerManager& parent) : ServerManager(parent)
	{}
	
	void registerUser(const JSON::Dict &credentials, int peer_id);
	User *logUserIn(const JSON::Dict &credentials, int peer_id);
    void checkTeamName(const JSON::Dict &data, int peer_id);
};

#endif // __USER_MANAGER_HPP