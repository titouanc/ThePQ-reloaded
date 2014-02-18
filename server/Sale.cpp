#include "Sale.hpp"

//thread creation
void * Sale::staticSaleStart(void * p){
	((Sale *) p)->saleStart();
	return NULL;
}

void Sale::start(){
	if(pthread_create(&_thread, NULL, Sale::staticSaleStart, this) != 0){
		throw "Error occured when creating sale thread";
	}
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

void Sale::resolveEndOfSale(){
	lock();
	if (_currentBidder != 0){
		_market->transfert(_owner,_currentBidder,_saleID);
	}
	else{} //Player not sold. Behaviour ?  
	_market->deleteSale(this);
	unlock();
}
	
void Sale::resolveEndOfTurn(){
	lock();
	++_turn;
	_canBidTeams = _turnTeams;
	_turnTeams.clear();
	_timeLeft = getTotalTime();
	save();
	unlock();
}
//mutexes

Sale::Sale(const JSON::Dict & json, PlayerMarket *market): _market(market), _currentBidder(0), 
_mymutex(PTHREAD_MUTEX_INITIALIZER), _playerPath("data/players/"), _marketPath("data/playerMarket/"), 
_owner(INT(json.get(net::MSG::TEAM_ID))), _saleID(INT(json.get(net::MSG::PLAYER_ID))),
_bidValue(INT(json.get(net::MSG::BID_VALUE))), _bidRatio(BIDRATIO), _turn(1), _timeLeft(FIRST_TURN), 
_repr(json) {
	save();
}



bool Sale::isSaler(int team_id){
	return (team_id == getOwner());
}

bool Sale::canBid(int team_id){
	for(int i;i<_canBidTeams.size();++i){
		if(_canBidTeams[i] == team_id){return true;}
	}
	return false;
}

void Sale::placeBid(int team_id, int bid_value){
	_currentBidder = team_id;
	_turnTeams.push_back(team_id);
	_bidValue = bid_value;
	updateDict();
}

JSON::Dict Sale::loadPlayerInfos(int player_id){
	JSON::Value* player = JSON::load(getPlayerPath().c_str());
	return *((JSON::Dict*)player);
}

void Sale::updateDict(int timer){
	_repr.set(net::MSG::BID_TIMER, timer);
}

void Sale::updateDict(){
	_repr.set(net::MSG::TEAMS_BIDDING, JSON::List());
	for(int i=0;i<_turnTeams.size();++i){LIST(_repr.get(net::MSG::TEAMS_BIDDING)).append(_turnTeams[i]);}
	_repr.set(net::MSG::CAN_BID_TEAMS, JSON::List());
	for(int i=0;i<_canBidTeams.size();++i){LIST(_repr.get(net::MSG::CAN_BID_TEAMS)).append(_canBidTeams[i]);}
	_repr.set(net::MSG::BID_VALUE, _bidValue);
	_repr.set(net::MSG::BID_TIMER, _timeLeft);
	_repr.set(net::MSG::TURN_NUMBER, _turn);
	_repr.set(net::MSG::CURRENT_BIDDER, _currentBidder);
}

void Sale::save(){
	updateDict();
	_repr.set(net::MSG::PLAYER, JSON::Dict());
	DICT(_repr.get(net::MSG::PLAYER)) = loadPlayerInfos(_saleID);
	_repr.save(getSalePath().c_str());	
}




