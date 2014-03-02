#ifndef __USER_DATA_HPP
#define __USER_DATA_HPP

#include <vector>
#include <string>
#include <model/Installation.hpp>
#include <model/Sale.hpp>
#include <model/Player.hpp>

struct UserData
{
	// Login
	void login(std::string name) { username = name; }
	void logout() {
		installations.clear();
		players.clear();
		username = ""; 
	}
	bool isLogged() { return username != ""; }

	// Cache
	std::vector<Installation> installations;
	std::vector<Player> players;
	std::string username;
};

#endif // __USER_DATA_HPP
