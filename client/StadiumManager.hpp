#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "User.hpp"
#include "Menu.hpp"
#include <model/Installation.hpp>

class StadiumManager
{
public:
	StadiumManager(net::ClientConnectionManager& connection, User& user);

	void displayMenu();
	
private:
	net::ClientConnectionManager& _connection;
	User& _user;
	
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	void printInstallationsList();
	void upgradeInstallation();
	void downgradeInstallation();
	std::vector<Installation> getInstallationsList();
};

#endif // __STADIUM_MANAGER_HPP
