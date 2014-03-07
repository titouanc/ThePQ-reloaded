#include "StadiumManager.hpp"

StadiumManager::StadiumManager(ClientManager const & parent) : ClientManager(parent)
{}

void StadiumManager::loadInstallations()
{
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	connection().send(query);

	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::INSTALLATIONS_LIST);
	JSON::Dict const & response = DICT(serverResponse);
	
	user().installations.clear();
	if (ISLIST(response.get("data")))
	{
		toFill = LIST(response.get("data"));
		for (size_t i = 0; i < toFill.len(); ++i)
		{
			user().installations.push_back(Installation::CAST(DICT(toFill[i])));
		}
	}
	delete serverResponse;
}

bool StadiumManager::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	connection().send(query);
	
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::INSTALLATION_UPGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISBOOL(received.get("data")))
	{
		ret = BOOL(received.get("data"));
	}
	delete serverResponse;
	return ret;
}

bool StadiumManager::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	connection().send(query);
	
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::INSTALLATION_DOWNGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	
	if (ISBOOL(received.get("data")))
	{
		ret = BOOL(received.get("data"));
	}
	
	delete serverResponse;
	return ret;
}

