#include "StadiumManager.hpp"

StadiumManager::StadiumManager(net::ClientConnectionManager &connection, User &user) :
	_connection(connection), _user(user)
{}

void StadiumManager::displayMenu()
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

void StadiumManager::printInstallationsList(){
	if (_user.installations.empty())
	{
		_user.installations = this->getInstallationsList();
	}
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < _user.installations.size(); ++i){
		cout << i << " - " << _user.installations[i].getName() << endl;
		cout << "      Level : 				" << _user.installations[i].getLevel() << endl;
		cout << "      Current Value : 		" << _user.installations[i].getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << _user.installations[i].getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << _user.installations[i].getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << _user.installations[i].getDowngradeRefunds() << endl;
	}
}

void StadiumManager::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl << ">";
	cin >> choice;
	if (choice < _user.installations.size())
	{
		if (upgradeInstallation(choice))
		{
			_user.installations[choice].upgrade();
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
	if (choice < _user.installations.size())
	{
		if (downgradeInstallation(choice))
		{
			_user.installations[choice].downgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

vector<Installation> StadiumManager::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATIONS_LIST);
	JSON::Dict const & response = DICT(serverResponse);
	
	vector<Installation> vec;
	if (ISLIST(response.get("data")))
	{
		toFill = LIST(response.get("data"));
		for (size_t i = 0; i < toFill.len(); ++i)
		{
			vec.push_back(DICT(toFill[i]));
		}
	}
	delete serverResponse;
	return vec;
}

bool StadiumManager::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATION_UPGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
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
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATION_DOWNGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
	}
	
	delete serverResponse;
	return ret;
}
