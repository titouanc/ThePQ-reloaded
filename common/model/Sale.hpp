#ifndef SALE_HPP
#define SALE_HPP 

#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <json/json.hpp>
#include <model/Player.hpp>

/*
TODO :
- Behaviour when player not sold ??
- CONST /!\
*/
#define FIRST_TURN 600 	//10 min
#define TURN_TIME 30
#define BIDRATIO 0.05 	//5%

class PlayerMarket;
class Sale{
	friend class PlayerMarket;
	friend std::ostream& operator<< (std::ostream&, const Sale&);
private:
	std::vector<std::string> _turnTeams;
	std::vector<std::string> _canBidTeams;
	int _bidValue;
	float _bidRatio;
	int _turn;
	std::string _currentBidder; 		//Team id currently winning bid
	std::string _owner;
	std::string _marketPath;
	std::string _playerPath;
	int _timeLeft;
	int _saleID;
	JSON::Dict _repr; 
	pthread_t _thread;
	pthread_mutex_t _mymutex;
	Player _player;
	bool _ended;

	static void * staticSaleStart(void * p);
	void saleStart();
	void start();
	void resolveEndOfTurn();
	void lock(){pthread_mutex_lock(&_mymutex);}
	void unlock(){pthread_mutex_unlock(&_mymutex);}
public:
	Sale();
	Sale(int bidValue, std::string owner, int id, const Player & player);
	Sale(const JSON::Dict & json);
	Sale(const JSON::Dict & json, const Player & player);
	Sale(const Sale & other);
	
	std::string getCurrentBidder(){return _currentBidder;}
	int getID(){return _saleID;}
	std::string getOwner(){return _owner;}
	JSON::Dict * getDict(){return &_repr;}
	std::string getSalePath(int id){return (_marketPath + "sale_" + std::to_string(id) + ".json");}
	std::string getPlayerPath(std::string owner){return (_playerPath + "users/" + owner + "/" + "players.json");}
	int getNextBidValue() const {return (_bidValue + (int)_bidValue*_bidRatio);}
	int getTotalTime(){
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	bool isOver(){return _ended;}
	bool isSaler(std::string username);
	bool allowedToBidForThisTurn(std::string username);
	void placeBid(std::string username, int bid_value);
	
	void updateDict();
	void updateDict(int timer);
	void save(std::string path);
};
#endif