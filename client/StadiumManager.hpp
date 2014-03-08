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
	virtual void onUpgradeInstallation();
	virtual void onDowngradeInstallation();
	virtual void onInstallationsLoad(JSON::List const & json);
};

#endif // __STADIUM_MANAGER_HPP
