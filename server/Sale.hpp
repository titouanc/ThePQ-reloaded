#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include <list>
#include <queue>
#include <ctime>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>

#define FIRST_TURN 600 //10 min
#define TURN_TIME 30
#define BIDRATIO 0.05 //5%
class Sale{
private:
	list<Team> _turnTeams;
	queue<Team> _bidTeams;
	int _bidValue;
	int _nextBidValue;
	float _bidRatio;
	int _turn;
	time_t _begin;
	Player _player;
public:
	Sale(): {}
	void save();
	Sale load(std::string playerid);
};


#endif