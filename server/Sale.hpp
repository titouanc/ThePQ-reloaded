#ifndef SALE_HPP
#define SALE_HPP 

#include <string>
#include <list>
#include <queue>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include "PlayerMarket.hpp"

/*
TODO :
- Behaviour when player not sold ??
- CONST /!\
*/
class PlayerMarket;
class Sale{
private:
	#define FIRST_TURN 600 	//10 min
	#define TURN_TIME 30
	#define BIDRATIO 0.05 	//5%
	vector<int> _turnTeams;		//Team id
	vector<int> _canBidTeams;	//Team id
	int _bidValue;
	float _bidRatio;
	int _turn;
	int _currentBidder; 		//Team id currently winning bid
	int _owner;
	int _timeLeft;
	int _saleID;
	std::string _marketPath;
	JSON::Dict _repr; 
	pthread_t _thread;
	pthread_mutex_t _mymutex;
	PlayerMarket *_market;

	static void * staticSaleStart(void * p);
	void saleStart();
	int getTotalTime();
	void resolveEndOfSale();
	void resolveEndOfTurn();

public:
	Sale(const JSON::Dict & json, PlayerMarket *market);
	std::string getSalePath(){return (_marketPath + "sale_" + std::to_string(_saleID) + ".json");}
	std::string getPlayerPath(){return (_playerPath + std::to_string(_saleID) + ".json");}
	void lock(){pthread_mutex_lock(&_mymutex);}
	void unlock(){pthread_mutex_unlock(&_mymutex);}
	int currentBidder(){return _currentBidder;}
	int getID(){return _saleID;}
	int getOwner(){return _owner;}
	JSON::Dict * getDict(){return &_repr;}
	int getNextBidValue(){return (_bidValue + (int)_bidValue*_bidRatio);}
	int getTotalTime(){
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	bool isSaler(int team_id);
	bool canBid(int team_id);
	void start();
	void placeBid(int team_id, int bid_value);
	JSON::Dict loadPlayerInfos(int player_id);
	void updateDict();
	void updateDict(int timer);
	void save();
};
#endif