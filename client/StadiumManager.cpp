#include "StadiumManager.hpp"

using namespace std;

StadiumManager::StadiumManager(Connection& connection): _connection(connection){
	loadInstallations();
}

void StadiumManager::printInstallationsList(){
	// TODO implement printInstallationsList
	cout << "Installations!" << endl;
	cout << "You have NOTHING" << endl; 
}

void StadiumManager::loadInstallations(){
	JSON::Dict received;
	_connection.getInstallationsList(received);
	cout << "Server response in load installations : " << endl;
	cout << received << endl;
}