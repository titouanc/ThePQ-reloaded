#include "FriendlyGameManager.hpp"

using namespace std;

vector<string> FriendlyGameManager::_connectedUsers;

void FriendlyGameManager::printUsersList(){
	cout << "Here are all the connected users : " << endl;
 	for (int i=0; i < _connectedUsers.len(); ++i)
 		cout << "  - " << _connectedUsers[i] << endl;
}

void FriendlyGameManager::loadUsersList(Connection *connection){
	connection->getConnectedUsersList(_connectedUsers);
}