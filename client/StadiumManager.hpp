#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP

#include <vector>
#include "ClientManager.hpp"
#include "Menu.hpp"
#include <model/Installation.hpp>

class StadiumManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	StadiumManager(ClientManager const & parent);
	void showMenu();
	void printInstallationsList();
	void upgradeInstallation();
	void downgradeInstallation();
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	void loadInstallations();
};

#endif // __STADIUM_MANAGER_HPP
