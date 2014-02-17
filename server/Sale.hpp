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

#define FIRST_TURN 600 	//10 min
#define TURN_TIME 30
#define BIDRATIO 0.05 	//5%
class Sale{
private:
	vector<Team> _turnTeams;
	queue<Team> _bidTeams;
	int _bidValue;
	float _bidRatio;
	int _turn;
	time_t _begin;
	Player _player;
	pthread_t thread;
	static void * staticSaleStart(void * p);
	void saleStart();
public:
	int getID();
	void start();
	Sale(const JSON::Dict & json);
	void save();
	Sale load(std::string playerid);
};

Sale::Sale(const JSON::Dict & json): _bidValue(INT(json->get("bidValue").value())), _bidRatio(BIDRATIO), 
	_turn(1), _begin(std::time(NULL), _player(json.get("player").value()){}

void Sale::start(){
	if(pthread_create(&thread, NULL, Sale::staticSaleStart, this) != 0){throw "Error occured when creating sale thread";
}

void * Sale::staticSaleStart(void * p){
	((Sale *) p)->saleStart();
	return NULL;
}

void Sale::saleStart(){
	if(_turn == 1){std::sleep(FIRST_TURN);}	//In case you load an existing sale (server crash...)
	while(_turnTeams.size() > 1){
		std::sleep(TURN_TIME);
		++_turn;
	}
}

#endif