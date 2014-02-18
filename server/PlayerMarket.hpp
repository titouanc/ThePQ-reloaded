#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include <stdio.h>
#include "Sale.hpp"
/*
TODO : 
- constructor loading all json files in memory (server crash ?)
- transfert when sale over (if player sold)
*/

class PlayerMarket{
	friend void Sale::resolveEndOfSale();
private:
	std::vector<Sale> _sales;
	std::string _marketPath;

	void delete(Sale * sale);
	void createSale(const JSON::Dict &json);
	void transfert(int from_team, int to_team, int player_id);
public:
	PlayerMarket();
	std::string getSalePath(int id);
	Sale * getSale(int id);
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict deletePlayer(const JSON::Dict &json);
	JSON::Dict bid(JSON::Dict &json);
};

#endif