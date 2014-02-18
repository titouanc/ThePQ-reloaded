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
- Behaviour when player not sold ??
- CONST /!\
*/
class PlayerMarket;

class Sale{
	friend void PlayerMarket::lockMutex(Sale* sale);
	friend void PlayerMarket::unlockMutex(Sale* sale);
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
	int _owner;
	std::string _marketPath;
	JSON::Dict _repr; 
	pthread_t _thread;
	pthread_mutex_t _mymutex;
	PlayerMarket *_market;

	static void * staticSaleStart(void * p);
	void saleStart();
	int getTotalTime(){
		if(_turn==1){return FIRST_TURN;}
		else{return TURN_TIME;}
	}
	void lock(){pthread_mutex_lock(&_mymutex);}
	void unlock(){pthread_mutex_unlock(&_mymutex);}
	void resolveEndOfSale(){
		lock();
		if (currentBidder != 0){
			_market->transfert(_owner,_currentBidder,_saleID);
		}
		else{} //Player not sold. Behaviour ?  
		_market->delete(this);
		unlock();
	}
	void resolveEndOfTurn(){
		lock();
		++_turn;
		_canBidTeams = _turnTeams;
		_turnTeams.clear();
		_timeLeft = getTotalTime();
		save();
		unlock();
	}

public:
	Sale(const JSON::Dict & json, PlayerMarket *market);
	std::string getSalePath(){return (_marketPath + "sale_" + std::to_string(_saleID) + ".json");}
	std::string getPlayerPath(){return (_playerPath + std::to_string(_saleID) + ".json");}
	bool isSaler(int team_id){return (team_id == getOwner());}
	bool canBid(int team_id){
		for(int i;i<_canBidTeams.size();++i){
			if(canBidTeams[i] == team_id){return true;}
		}
		return false;
	}
	int currentBidder(){return _currentBidder;}
	int getID(){return _saleID;}
	int getOwner(){return _owner;}
	JSON::Dict * getDict(){return &_repr;}
	void start();

	void placeBid(int team_id, int bid_value){
		_currentBidder = team_id;
		_turnTeams.push_back(team_id);
		_bidValue = bid_value;
		updateDict();
	}

	JSON::Dict loadPlayerInfos(int player_id){
		JSON::Value* player = JSON::load(getPlayerPath().c_str());
		return *((JSON::Dict*)player);
	}

	int getNextBidValue(){return (_bidValue + (int)_bidValue*_bidRatio);}

	void updateDict(int timer){
		_repr.set(net::MSG::BID_TIMER, timer);
	}

	void updateDict(){
		_repr.set(net::MSG::TEAMS_BIDDING, JSON::List());
		for(int i=0;i<_turnTeams.size();++i){LIST(_repr.get(net::MSG::TEAMS_BIDDING)).append(_turnTeams[i]);}
		_repr.set(net::MSG::CAN_BID_TEAMS, JSON::List());
		for(int i=0;i<_canBidTeams.size();++i){LIST(_repr.get(net::MSG::CAN_BID_TEAMS)).append(_canBidTeams[i]);}
		_repr.set(net::MSG::BID_VALUE, _bidValue);
		_repr.set(net::MSG::BID_TIMER, _timeLeft);
		_repr.set(net::MSG::TURN_NUMBER, _turn);
		_repr.set(net::MSG::CURRENT_BIDDER, _currentBidder);
	}

	void save(){
		updateDict();
		_repr.set(net::MSG::PLAYER, JSON::Dict());
		DICT(_repr.get(net::MSG::PLAYER)) = loadPlayerInfos(_saleID);
		_repr.save(getSalePath().c_str());	
	}
};

Sale::Sale(const JSON::Dict & json, PlayerMarket *market): _market(market), _currentBidder(0), _mymutex(PTHREAD_MUTEX_INITIALIZER), _playerPath("data/players/"),
 _marketPath("data/playerMarket/"), _owner(INT(json.get(net::MSG::TEAM_ID))), _saleID(INT(json.get(net::MSG::PLAYER_ID))),
 _bidValue(INT(json.get(net::MSG::BID_VALUE))), _bidRatio(BIDRATIO), _turn(1), _timeLeft(FIRST_TURN), _repr(json) {
	save();
}

void Sale::start(){
	if(pthread_create(&_thread, NULL, Sale::staticSaleStart, this) != 0){throw "Error occured when creating sale thread";}
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
			updateDict(_timeLeft);
		}
		resolveEndOfTurn();
	}
	resolveEndOfSale();
}

#endif