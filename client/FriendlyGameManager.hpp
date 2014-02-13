#ifndef __FRIENDLY_GAME_MANAGER_HPP
#define __FRIENDLY_GAME_MANAGER_HPP

#include <vector>
#include <string>
#include "Connection.hpp"

using namespace std;

class FriendlyGameManager {
public:
	static void printUsersList();
	static void loadConnectedUsersList(Connection* connection);
private:
	static std::vector<std::string> _connectedUsers;
};

#endif