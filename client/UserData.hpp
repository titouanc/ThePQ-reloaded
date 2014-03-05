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

	~UserData()
	{
		for (size_t i = 0; i < installations.size(); ++i)
		{
			delete installations[i];
		}
	}

	// Cache
	std::vector<Installation*> installations;
	std::vector<Player> players;
	std::string username;
	int funds;
};

#endif // __USER_DATA_HPP
