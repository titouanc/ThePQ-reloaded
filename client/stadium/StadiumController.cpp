#include "StadiumController.hpp"

///Constructor
StadiumController::StadiumController(ClientController const & parent) : ClientController(parent)
{}

/**
 * Method handling the queries to the server for the installations
 */
void StadiumController::loadInstallations()
{
	say(net::MSG::INSTALLATIONS_LIST, JSON::String(""));
}

/**
 * Method handling queries to the server for upgrades of installations
 */
void StadiumController::upgradeInstallation(size_t i)
{
	say(net::MSG::INSTALLATION_UPGRADE, JSON::Integer(i));
}

/**
 * Method handling queries to the server for downgrades of installations
 */
void StadiumController::downgradeInstallation(size_t i)
{
	say(net::MSG::INSTALLATION_DOWNGRADE, JSON::Integer(i));
}

/**
 * Method handling queries from the server
 * @param string : type of the query
 * @param JSON::Value : query from the server
 */
void StadiumController::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::INSTALLATION_UPGRADE)
	{
		if(ISBOOL(data))
			onUpgradeInstallation(BOOL(data));
	}
	else if (type == net::MSG::INSTALLATION_DOWNGRADE)
	{
		if (ISBOOL(data))
			onDowngradeInstallation(BOOL(data));
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

/**
 * Method handling upgrades of installations
 */
void StadiumController::onUpgradeInstallation(bool success)
{
	if (success == true)
		loadInstallations();
}

/**
 * Method handling downgrades of installations
 */
void StadiumController::onDowngradeInstallation(bool success)
{
	if (success == true)
		loadInstallations();
}

