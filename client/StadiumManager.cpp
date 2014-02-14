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
	for (vector<Installation>::const_iterator it=_installations.begin(); it!=_installations.end(); it++){
		cout << "  - " << it->getName() << endl;
		cout << "      Level : 				" << it->getLevel() << endl;
		cout << "      Upgrade Cost : 		" << it->getUpgradeCost() << endl;
		cout << "      Downgrade Refunds : 	" << it->getDowngradeRefunds() << endl;
	}
}

void StadiumManager::upgradeInstallation()
{
	char choice;
	cout << "Enter the number of the installation you want to upgrade" << endl;
	cin >> choice;
	if (choice >= 0 and choice < vector.length())
	{
		// TODO _connection.upgradeInstallation(vector[i]);
	}
}

void StadiumManager::downgradeInstallation()
{
	
}
