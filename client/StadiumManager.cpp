#include "StadiumManager.hpp"

StadiumManager::StadiumManager(ClientManager const & parent) : ClientManager(parent)
{}

void StadiumManager::loadInstallations()
{
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	connection().send(query);
}

void StadiumManager::upgradeInstallation(size_t i)
{
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	connection().send(query);
}

void StadiumManager::downgradeInstallation(size_t i)
{
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	connection().send(query);
}

void StadiumManager::treatMessage(std::string const & type, JSON::Value const *)
{
	if (type == net::MSG::INSTALLATION_UPGRADE)
	{
		onUpgradeInstallation();
	}
	else if (type == net::MSG::INSTALLATION_DOWNGRADE)
	{
		onUpgradeInstallation();
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


