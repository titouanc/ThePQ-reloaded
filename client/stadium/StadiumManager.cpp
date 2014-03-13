#include "StadiumManager.hpp"

StadiumManager::StadiumManager(ClientManager const & parent) : ClientManager(parent)
{}

void StadiumManager::loadInstallations()
{
	say(net::MSG::INSTALLATIONS_LIST, JSON::String(""));
}

void StadiumManager::upgradeInstallation(size_t i)
{
	say(net::MSG::INSTALLATION_UPGRADE, JSON::Integer(i));
}

void StadiumManager::downgradeInstallation(size_t i)
{
	say(net::MSG::INSTALLATION_DOWNGRADE, JSON::Integer(i));
}

void StadiumManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::INSTALLATION_UPGRADE)
	{
		onUpgradeInstallation();
	}
	else if (type == net::MSG::INSTALLATION_DOWNGRADE)
	{
		onDowngradeInstallation();
	}
	else if (type == net::MSG::INSTALLATIONS_LIST)
	{
		JSON::List const & installs = LIST(data);
		user().installations.clear();
		for (size_t i = 0; i < installs.len(); ++i)
			user().installations.push_back(Installation::CAST(DICT(installs[i])));
		onInstallationsLoad();
	}
}

void StadiumManager::onUpgradeInstallation()
{
	loadInstallations();
}

void StadiumManager::onDowngradeInstallation()
{
	loadInstallations();
}

