#ifndef __TEAM_MANAGER_HPP
#define __TEAM_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include "Menu.hpp"

class TeamManager
{
public:
	TeamManager(net::ClientConnectionManager& connection, UserData& user);
	
	void displayMenu();
	void printPlayers();
	void loadPlayers();
	
private:
	net::ClientConnectionManager& _connection;
	UserData& _user;
};

#endif // __TEAM_MANAGER_HPP
