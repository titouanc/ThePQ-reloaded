#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include <vector>
#include "UserData.hpp"
#include "Menu.hpp"
#include "ClientManager.hpp"

class TeamManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	TeamManager(ClientManager const & parent);
private:
};

#endif // __TEAM_MANAGER_HPP
