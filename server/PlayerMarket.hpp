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
	friend void * saleManager(void *); 		//Thread deleting the ended sales
	friend void * saleGenerator(void *); 	//Thread generating players
private:
	Server *_server;
	std::vector<Sale*> _sales;
	pthread_t _manager;
	pthread_t _generator;
	bool _runManager;
	bool _runGenerator;
	pthread_mutex_t _locker;

	void startManager();
	void startGenerator();
	void lockMarket(){pthread_mutex_lock(&_locker);}
	void unlockMarket(){pthread_mutex_unlock(&_locker);}
	void transfert(std::string,std::string,int,int,Sale* =NULL);
	void createSale(int, int, Player&, std::string);
	void resolveEndOfSale(Sale * sale);
	Sale * getSale(int id);
	/* Used to send notifications (when a sale ends) */
	void sendMessageToUser(std::string, const JSON::Dict&);
public:
	PlayerMarket(Server*);
	~PlayerMarket();
	/* Functions called by server, returns what will be send to the user */
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict bid(const JSON::Dict &json);
	
};

#endif
