#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP 

#include <string>
#include <vector>
#include "Connection.hpp"
#include <model/Installation.hpp>
#include <json/json.hpp>

class StadiumManager {
public:
	StadiumManager(Connection* connection);
	void printInstallationsList();
	
	void upgradeInstallation();
	void downgradeInstallation();

private:
	void loadInstallations();
	
	std::vector<Installation> _installations;
	Connection *_connection;
};

#endif
