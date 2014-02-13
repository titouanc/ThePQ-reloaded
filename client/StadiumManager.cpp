#include "StadiumManager.hpp"

using namespace std;

JSON::Dict StadiumManager::_installations;
Connection StadiumManager::_connection;

void StadiumManager::printInstallationsList(){
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (JSON::Dict::const_iterator it=_installations.begin(); it!=_installations.end(); it++){
		cout << "  - " << it->first << endl;
	}
}

void StadiumManager::loadInstallations(Connection& connection){
	_connection = connection;
	_connection.getInstallationsList(_installations);
}
