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
- CONST /!\
*/
#define FIRST_TURN 30 	//10 min
#define TURN_TIME 30
#define BIDRATIO 0.05 	//5%

class PlayerMarket;
class Sale{
	friend std::ostream& operator<< (std::ostream&, const Sale&);
private:
	std::vector<std::string> _turnTeams;
	std::vector<std::string> _canBidTeams;
	int _bidValue;
	float _bidRatio;
	int _turn;
	std::string _currentBidder; 		//User currently winning sale
	std::string _owner;
	std::string _marketPath;
	std::string _playerPath;
	int _timeLeft;
	int _saleID;
	pthread_t _thread;
	pthread_mutex_t _mymutex;
	Player _player;
	bool _ended;

	static void * staticSaleStart(void * p);
	void saleStart();
	
	void resolveEndOfTurn();
	void lock(){pthread_mutex_lock(&_mymutex);}
	void unlock(){pthread_mutex_unlock(&_mymutex);}
public:
	Sale();
	Sale(int bidValue, std::string owner, int id, const Player & player);
	Sale(const JSON::Dict & json);
	Sale(const JSON::Dict & json, const Player & player);
	Sale(const Sale & other);
	operator JSON::Dict();
	void start();
	std::string getCurrentBidder(){return _currentBidder;}
	int getID(){return _saleID;}
	std::string getOwner() const {return _owner;}
	std::string getSalePath(int id) const {return (_marketPath + "sale_" + std::to_string(id) + ".json");}
	std::string getPlayerPath(std::string owner) const {return (_playerPath + "users/" + owner + "/" + "players.json");}
	int getNextBidValue() const {return (_bidValue + (int)_bidValue*_bidRatio);}
	int getTotalTime() const {
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	int getBidValue() const {return _bidValue;}
	bool isOver() const {return _ended;}
	bool isSaler(std::string username) const;
	bool allowedToBidForThisTurn(std::string username) const;
	void placeBid(std::string username, int bid_value);

	void save(std::string path);
	void load(std::string path, int id);
};
#endif