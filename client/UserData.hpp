#ifndef __USER_DATA_HPP
#define __USER_DATA_HPP

#include <vector>
#include <string>
#include <model/Installation.hpp>
#include <model/Sale.hpp>
#include <model/Player.hpp>
#include <model/Championship.hpp>

struct UserData
{
	// Login
	void logout() {
		installations.clear();
		players.clear();
		username = ""; 
		teamname = "";
		funds = -1;
		acPoints = -1;//new addition
		fame=-1;
	}
	bool isLogged() { return username != ""; }

	UserData(JSON::Dict const & json = JSON::Dict())
	{
		if (ISSTR(json.get(net::MSG::USERNAME)))
			username = STR(json.get(net::MSG::USERNAME)).value();
		
		if (ISSTR(json.get("teamname")))
			teamname = STR(json.get("teamname")).value();
		
		if (ISINT(json.get("funds")))
			funds = INT(json.get("funds"));
	}
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
	Championship joinedChamp;
	std::string username;
	std::string teamname;
	int funds;
	int acPoints;//new addition
	int fame;
};

#endif // __USER_DATA_HPP
