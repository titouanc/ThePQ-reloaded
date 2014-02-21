#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <stdio.h>
#include <model/Sale.hpp>
#include <model/Player.hpp>
#include <pthread.h>

#include "Server.hpp"

/*
TODO : 
- constructor loading all json files in memory (server crash ?)
- transfert when sale over (if player sold)
*/
class Server;
class Sale;
class PlayerMarket{
	friend void * saleChecker(void *);
private:
	Server *_server;
	std::vector<Sale*> _sales;
	std::string _marketPath;
	std::string _playerPath;
	pthread_t _thread;
	bool _runChecker;
	pthread_mutex_t _deleting;

	void startChecker();
	void deletingLock(){pthread_mutex_lock(&_deleting);}
	void deletingUnlock(){pthread_mutex_unlock(&_deleting);}
public:
	PlayerMarket(Server*);
	~PlayerMarket();
	void createSale(const JSON::Dict &json);
	void transfert(Sale * sale);
	void loadSales();
	Sale * getSale(int id);
	Player loadPlayerInfos(std::string username, int id);
	std::string getPlayersPath(std::string owner){return (_playerPath + "users/" + owner + "/" + "players.json");}
	void sendMessageToUser(std::string, const JSON::Dict&);
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict deletePlayer(const JSON::Dict &json);
	JSON::Dict bid(const JSON::Dict &json);
	
};

#endif