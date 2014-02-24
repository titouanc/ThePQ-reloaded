#ifndef __USER_HPP
#define __USER_HPP

#include <vector>
#include <string>
#include <model/Installation.hpp>
#include <model/Sale.hpp>
#include <model/Player.hpp>

class User
{
public:
	User();
	
	void login(std::string username) { _username = username; }
	void logout() { _username = ""; }
	bool isLogged() { return _username != ""; }
	std::string getUsername() { return _username; }
	
private:
	//~ std::vector<Installation> _installations;
	//~ std::vector<Sale> _playersOnSale;
	//~ std::vector<Player> _players;
	std::string _username;
	
};

#endif // __USER_HPP
