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

class Server;
class Sale;
class PlayerMarket{
	friend void * saleChecker(void *); //Thread deleting the ended sales
private:
	Server *_server;
	std::vector<Sale*> _sales;
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
	void resolveEndOfSale(Sale * sale);
	void transfert(std::string,std::string,int,int);
	Sale * getSale(int id);
	void sendMessageToUser(std::string, const JSON::Dict&);
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict deletePlayer(const JSON::Dict &json);
	JSON::Dict bid(const JSON::Dict &json);
	
};

#endif
