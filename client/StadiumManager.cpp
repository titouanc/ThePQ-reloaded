#include "StadiumManager.hpp"

using namespace std;

StadiumManager::StadiumManager(Connection& connection): _connection(connection){
	loadInstallations();
}

void StadiumManager::printInstallationsList(){
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (JSON::Dict::const_iterator it=_installations.begin(); it!=_installations.end(); it++){
		cout << "  - " << it->first << endl;
	}
}

void StadiumManager::loadInstallations(){
	_connection.getInstallationsList(_installations);
}