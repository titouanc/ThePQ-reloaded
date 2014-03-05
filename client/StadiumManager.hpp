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
	void showMenu();
	
private:
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	void printInstallationsList();
	void upgradeInstallation();
	void downgradeInstallation();
	void loadInstallations();
};

#endif // __STADIUM_MANAGER_HPP
