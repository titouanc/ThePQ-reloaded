#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP 

#include <string>
#include <map>
#include "Connection.hpp"
#include "json.hpp"
#include "Installation.hpp"

using namespace std;

class StadiumManager {
public:

	static void printInstallationsList();
	static void loadInstallations(Connection* connection);

private:
	static JSON::Dict _installations;
	static Connection *_connection;
};

#endif
