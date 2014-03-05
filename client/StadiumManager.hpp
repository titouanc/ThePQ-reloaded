#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include "Menu.hpp"
#include <model/Installation.hpp>

class StadiumManager
{
public:
	StadiumManager(net::ClientConnectionManager& connection, UserData& user);
	
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	void loadInstallations();
	
private:
	net::ClientConnectionManager& _connection;
	UserData& _user;
};

#endif // __STADIUM_MANAGER_HPP
