#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP

#include "ServerManager.hpp"

class StadiumManager : public ServerManager
{
public:
	using ServerManager::ServerManager;
	StadiumManager(const ServerManager & parent) : ServerManager(parent)
	{}

	void sendInstallationsList(int peer_id);
	void upgradeInstallation(int peer_id, size_t i);
	void downgradeInstallation(int peer_id, size_t i);
};

#endif // __STADIUM_MANAGER_HPP