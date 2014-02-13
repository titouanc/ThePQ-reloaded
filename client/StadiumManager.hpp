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

	static void printInstallationsList();
	static void loadInstallations(Connection& connection);

private:
	static JSON::Dict _installations;
	static Connection _connection;
};

#endif
