#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "User.hpp"
#include "Menu.hpp"

class TeamManager
{
public:
	TeamManager(net::ClientConnectionManager& connection, User& user);
	
private:
	void loadPlayers();
	net::ClientConnectionManager& _connection;
	User& _user;
};

#endif // __TEAM_MANAGER_HPP
