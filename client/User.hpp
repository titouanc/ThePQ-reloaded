#ifndef __USER_HPP
#define __USER_HPP

#include <vector>
#include <string>
#include <model/Installation.hpp>
#include <model/Sale.hpp>
#include <model/Player.hpp>

struct User
{
	// Login
	void login(std::string username) { username = username; }
	void logout() { username = ""; }
	bool isLogged() { return username != ""; }

	// Cache
	std::vector<Installation> installations;
	//~ std::vector<Sale> _playersOnSale;
	std::vector<Player> players;
	std::string username;
};

#endif // __USER_HPP
