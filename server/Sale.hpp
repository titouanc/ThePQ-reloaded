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

/*todo : à la construction : ajouter Player Dict dans _repr
- save
- load
- endofsale
- CONST /!\
*/
#define FIRST_TURN 600 	//10 min
#define TURN_TIME 30
#define BIDRATIO 0.05 	//5%
class Sale{
private:
	vector<int> _turnTeams;	//Team id
	queue<int> _bidTeams;	//Team id
	int _bidValue;
	float _bidRatio;
	int _turn;
	int _currentBidder; //Team id currenty winning bid
	int _owner;
	int _timeLeft;
	int _saleID;
	int _owner;
	std::string _marketPath;
	JSON::Dict _repr; 
	pthread_t thread;

	static void * staticSaleStart(void * p);
	void saleStart();
	int getTotalTime(){
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	void resolveEndOfSale();
public:
	Sale(const JSON::Dict & json);
	std::string getSalePath(int id){return (_marketPath + "sale_" + std::to_string(_saleID) + ".json");}
	bool isSaler(int team_id){return (team_id == getOwner());}
	int getID(){return _saleID;}
	int getOwner(){return _owner;}
	JSON::Dict * getDict(){return &_repr;}
	void start();

	void save(){
		_repr.set(net::MSG::TEAMS_BIDDING, JSON::List());
		for(int i=0;i<_turnTeams.size();++i){LIST(_repr.get(net::MSG::TEAMS_BIDDING)).append(_turnTeams[i]);}
		_repr.set(net::MSG::BID_VALUE, _bidValue);
		_repr.set(net::MSG::TURN_NUMBER, _turn);
		_repr.set(net::MSG::BID_TIMER, _timeLeft);
		_repr.set(net::MSG::CURRENT_BIDDER, _currentBidder);
		_repr.set(net::MSG::PLAYER, JSON::Dict());
		DICT(_repr.get(net::MSG::PLAYER)) = getPlayerInfos(_saleID);
		_repr.save(getSalePath().c_str());	
	}
};

Sale::Sale(const JSON::Dict & json): _marketPath("data/playerMarket/"), _owner(INT(json.get(net::MSG::TEAM_ID))), _saleID(INT(json.get(net::MSG::PLAYER_ID))),
 _bidValue(INT(json.get(net::MSG::BID_VALUE))), _bidRatio(BIDRATIO), _turn(1), _timeLeft(FIRST_TURN), _repr(json) {
	/*
	*_repr(json) already copied : 
	*	- TEAM_ID;
	*	- PLAYER_ID;
	*	- BID_VALUE.
	*/
	
	save();
}

void Sale::start(){
	if(pthread_create(&thread, NULL, Sale::staticSaleStart, this) != 0){throw "Error occured when creating sale thread";}
}

void * Sale::staticSaleStart(void * p){
	((Sale *) p)->saleStart();
	return NULL;
}

void Sale::saleStart(){
	while(_turnTeams.size() > 1){
		for(int i=0; i<getTotalTime(); ++i){
			sleep(1);
			--_timeLeft;
		}

	resolveEndOfSale();
}

#endif