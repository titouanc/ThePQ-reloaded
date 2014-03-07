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
	void upgradeInstallation(size_t i);
	void downgradeInstallation(size_t i);
	void loadInstallations();
	void treatMessage(std::string const & type, JSON::Value const * data);
	void onInstallationsList(JSON::List const & installs);
	void onUpgradeInstallation();
	void onDowngradeInstallation();
};

#endif // __STADIUM_MANAGER_HPP
