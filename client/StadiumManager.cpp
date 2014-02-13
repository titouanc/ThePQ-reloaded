#include "StadiumManager.hpp"

using namespace std;

vector<Installation> StadiumManager::_installations;
Connection *StadiumManager::_connection;

void StadiumManager::setConnection(Connection* connection)
{
	_connection = connection;	
}

void StadiumManager::printInstallationsList(){
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (vector<Installation>::const_iterator it=_installations.begin(); it!=_installations.end(); it++){
		cout << "  - " << it->getName() << endl;
	}
}

void StadiumManager::loadInstallations(){
	_installations = _connection->getInstallationsList();
}
