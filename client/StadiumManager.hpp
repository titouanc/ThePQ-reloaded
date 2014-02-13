#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP 

#include <string>
#include <map>
#include "Installation.hpp"

using namespace std;

class StadiumManager {
public:
	StadiumManager();

	void printInstallationsList();
private:
	map<string, Installation> _installations;
};

#endif