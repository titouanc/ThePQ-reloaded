#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP 

#include <string>
#include <map>
#include "Installation.hpp"
#include "Connection.hpp"
#include "../common/json/json.hpp"

using namespace std;

class StadiumManager {
public:
	StadiumManager(Connection& connection);

	void printInstallationsList();
	void loadInstallations();

private:
	map<string, Installation> _installations;
	Connection _connection;
};

#endif