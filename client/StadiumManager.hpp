#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP 

#include <string>
#include <vector>
#include "Connection.hpp"
#include <model/Installation.hpp>
#include <json/json.hpp>

class StadiumManager {
public:
	static void setConnection(Connection* connection);
	static void printInstallationsList();
	static void loadInstallations();
	//~ static void upgradeInstallation(Installation i);
	//~ static void downgradeInstallation(Installation i);

private:
	static std::vector<Installation> _installations;
	static Connection *_connection;
};

#endif
