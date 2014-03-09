#ifndef SALE_HPP
#define SALE_HPP 

#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <json/json.hpp>
#include <model/Player.hpp>
#include <utility> //std::pair

class Sale{
	friend std::ostream& operator<< (std::ostream&, const Sale&);
private:
	std::vector<std::string> _turnTeams;
	std::vector<std::string> _canBidTeams;
	int _bidValue;
	float _bidRatio;
	int _turn;
	std::string _currentBidder; 
	std::string _owner;
	int _timeLeft;
	int _saleID;
	Player _player;
	bool _ended;
public:
	Sale();
	Sale(int bidValue, std::string owner, int id, const Player & player);
	Sale(const JSON::Dict & json);
	Sale(const JSON::Dict & json, const Player & player);
	Sale(const Sale & other);
	operator JSON::Dict();
	void resolveEndOfTurn();
	std::string getCurrentBidder() const {return _currentBidder;}
	int getID() const {return _saleID;}
	std::string getOwner() const {return _owner;}
	int getNextBidValue() const {return (_bidValue + (int)_bidValue*_bidRatio);}
	int getTimeLeft() const {return _timeLeft;}
	void decTime(){
		if(_timeLeft > 0)
			--_timeLeft;
	}
	int getTotalTime() const {
		if(_turn==1){return gameconfig::SALE_FIRST_TURN_TIME;}
		else{return gameconfig::SALE_TURN_TIME;}
	}
	Player const & getPlayer() const {return _player;}
	int getBidValue() const {return _bidValue;}
	bool isOver() const {return _ended;}
	bool isSaler(std::string username) const;
	bool allowedToBidForThisTurn(std::string username) const;
	std::pair<std::string,int> placeBid(std::string username, int bid_value);

	void save();
	void load();
};
#endif