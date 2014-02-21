#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <stdio.h>
#include <model/Sale.hpp>
#include <model/Player.hpp>
#include <pthread.h>

/*
TODO : 
- constructor loading all json files in memory (server crash ?)
- transfert when sale over (if player sold)
*/
class Sale;
class PlayerMarket{
	friend void * saleChecker(void *);
private:
	std::vector<Sale*> _sales;
	std::string _marketPath;
	std::string _playerPath;
	pthread_t _thread;
	bool _runChecker;

	void startChecker();
public:
	PlayerMarket();
	~PlayerMarket(){_runChecker = false;}
	//void removeSale(Sale * sale);//modif
	void createSale(const JSON::Dict &json);
	void transfert(Sale * sale);
	Sale * getSale(int id);
	Player loadPlayerInfos(std::string username, int id);
	std::string getPlayersPath(std::string owner){return (_playerPath + "users/" + owner + "/" + "players.json");}

	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict deletePlayer(const JSON::Dict &json);
	JSON::Dict bid(const JSON::Dict &json);
	
};

#endif