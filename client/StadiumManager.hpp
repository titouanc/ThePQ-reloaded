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
	
	static void upgradeInstallation(size_t i);
	static void downgradeInstallation(size_t i);

private:
	void loadInstallations();
	
	std::vector<Installation> _installations;
	Connection *_connection;
};

#endif
