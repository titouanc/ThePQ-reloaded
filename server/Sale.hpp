#ifndef SALE_HPP
#define SALE_HPP 

#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <json/json.hpp>
#include "PlayerMarket.hpp"
#include <model/Player.hpp>

/*
TODO :
- Behaviour when player not sold ??
- CONST /!\
*/
class PlayerMarket;
class Sale{
	friend class PlayerMarket;
private:
	#define FIRST_TURN 600 	//10 min
	#define TURN_TIME 30
	#define BIDRATIO 0.05 	//5%
	std::vector<std::string> _turnTeams;		//username
	std::vector<std::string> _canBidTeams;		//username
	int _bidValue;
	float _bidRatio;
	int _turn;
	std::string _currentBidder; 		//Team id currently winning bid
	std::string _owner;
	int _timeLeft;
	int _saleID;
	std::string _marketPath;
	std::string _playerPath;
	JSON::Dict _repr; 
	pthread_t _thread;
	pthread_mutex_t _mymutex;
	PlayerMarket *_market;

	static void * staticSaleStart(void * p);
	void saleStart();
	void start();
	void resolveEndOfSale();
	void resolveEndOfTurn();
	void lock(){pthread_mutex_lock(&_mymutex);}
	void unlock(){pthread_mutex_unlock(&_mymutex);}
public:
	Sale(const JSON::Dict & json, PlayerMarket *market);
	std::string getSalePath(){return (_marketPath + "sale_" + std::to_string(_saleID) + ".json");}
	std::string getPlayerPath(){return (_playerPath + "users/" + _owner + "/" + "players.json");}
	std::string getCurrentBidder(){return _currentBidder;}
	int getID(){return _saleID;}
	std::string getOwner(){return _owner;}
	JSON::Dict * getDict(){return &_repr;}
	int getNextBidValue(){return (_bidValue + (int)_bidValue*_bidRatio);}
	int getTotalTime(){
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	bool isSaler(std::string username);
	bool allowedToBidForThisTurn(std::string username);
	void placeBid(std::string username, int bid_value);
	Player loadPlayerInfos(std::string username, int id);
	void updateDict();
	void updateDict(int timer);
	void save();
};
#endif