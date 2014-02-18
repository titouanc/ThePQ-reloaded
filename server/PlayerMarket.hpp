#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <stdio.h>
#include "Sale.hpp"
/*
TODO : 
- constructor loading all json files in memory (server crash ?)
- transfert when sale over (if player sold)
*/
class Sale;
class PlayerMarket{
private:
	std::vector<Sale> _sales;
	std::string _marketPath;
public:
	PlayerMarket();
	void deleteSale(Sale * sale);
	void createSale(const JSON::Dict &json);
	void transfert(int from_team, int to_team, int player_id);
	std::string getSalePath(int id);
	Sale * getSale(int id);
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict deletePlayer(const JSON::Dict &json);
	JSON::Dict bid(const JSON::Dict &json);

};

#endif