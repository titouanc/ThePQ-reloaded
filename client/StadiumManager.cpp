#include "StadiumManager.hpp"

StadiumManager::StadiumManager(ClientManager const & parent) : ClientManager(parent)
{}

void StadiumManager::showMenu()
{
	Menu _menu;
	_menu.addToDisplay("    - view your installations\n");
	_menu.addToDisplay("    - upgrade an installation\n");
	_menu.addToDisplay("    - downgrade an installation\n");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				printInstallationsList();
				break;
			case 2:
				upgradeInstallation();
				break;
			case 3:
				downgradeInstallation();
				break;
			default:
				break;
		}
	}
	while (option != 4);
}

void StadiumManager::printInstallationsList()
{
	if (user().installations.empty())
	{
		loadInstallations();
	}
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < user().installations.size(); ++i){
		cout << i << " - " << user().installations[i]->getName() << endl;
		cout << "      Level : 				" << user().installations[i]->getLevel() << endl;
		cout << "      Current Value : 		" << user().installations[i]->getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << user().installations[i]->getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << user().installations[i]->getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << user().installations[i]->getDowngradeRefunds() << endl;
	}
}

void StadiumManager::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl << ">";
	cin >> choice;
	if (choice < user().installations.size())
	{
		if (upgradeInstallation(choice))
		{
			user().installations[choice]->upgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void StadiumManager::downgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to downgrade" << endl << ">";
	cin >> choice;
	if (choice < user().installations.size())
	{
		if (downgradeInstallation(choice))
		{
			user().installations[choice]->downgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

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

