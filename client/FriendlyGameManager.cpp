#include "FriendlyGameManager.hpp"

using namespace std;

vector<string> FriendlyGameManager::_connectedUsers;

void FriendlyGameManager::printConnectedUsersList(){
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < _connectedUsers.size(); ++i)
 		cout << "  - " << _connectedUsers[i] << endl;
}

void FriendlyGameManager::loadConnectedUsersList(Connection *connection){
	connection->getConnectedUsersList(_connectedUsers);
}