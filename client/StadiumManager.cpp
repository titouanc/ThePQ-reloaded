#include "StadiumManager.hpp"

using namespace std;

StadiumManager::StadiumManager(Connection* connection)
{
	_connection = connection;	
	loadInstallations();
}

void StadiumManager::loadInstallations(){
	_installations = _connection->getInstallationsList();
}

void StadiumManager::printInstallationsList(){
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < _installations.size(); ++i){
		cout << "  - " << _installations[i].getName() << endl;
		cout << "      Level : 				" << _installations[i].getLevel() << endl;
		cout << "      Upgrade Cost : 		" << _installations[i].getUpgradeCost() << endl;
		cout << "      Downgrade Refunds : 	" << _installations[i].getDowngradeRefunds() << endl;
	}
}

void StadiumManager::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl;
	cin >> choice;
	if (choice < _installations.size())
	{
		// TODO _connection.upgradeInstallation(vector[i]);
	}
}

void StadiumManager::downgradeInstallation()
{
	
}
