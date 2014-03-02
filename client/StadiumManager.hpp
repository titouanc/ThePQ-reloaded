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

	void showMenu();
	
private:
	net::ClientConnectionManager& _connection;
	UserData& _user;
	
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	void printInstallationsList();
	void upgradeInstallation();
	void downgradeInstallation();
	void loadInstallations();
};

#endif // __STADIUM_MANAGER_HPP
