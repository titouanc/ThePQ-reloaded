#include "Sale.hpp"
#include <Constants.hpp>

//Thread

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

//Sale managers

void Sale::resolveEndOfSale(){
	lock();
	if (! _currentBidder.empty()){
		_market->transfert(_owner,_currentBidder,_saleID);
	}
	else{} //Player not sold. Behaviour ?  
	_market->removeSale(this);//modif
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

Sale::Sale(const JSON::Dict & json, PlayerMarket *market): 
_turnTeams(), _canBidTeams(), _bidValue(INT(json.get(net::MSG::BID_VALUE))),_bidRatio(BIDRATIO),
_turn(1),_currentBidder(""),_owner(STR(json.get(net::MSG::USERNAME)).value()),_timeLeft(FIRST_TURN), 
_saleID(INT(json.get(net::MSG::PLAYER_ID))), _marketPath("data/playerMarket/"),_playerPath("data/"), 
_repr(json), _thread(), _mymutex(PTHREAD_MUTEX_INITIALIZER), _market(market)
{
	save();
}



//Test functions
bool Sale::isSaler(std::string username){
	return (username == getOwner());
}

bool Sale::allowedToBidForThisTurn(std::string username){
	if(_turn == 1){return true;}
	else{
		for(size_t i;i<_canBidTeams.size();++i){
			if(_canBidTeams[i] == username){return true;}
		}
	}
	return false;
}

//Services
void Sale::placeBid(std::string username, int bid_value){
	_currentBidder = username;
	_turnTeams.push_back(username);
	_bidValue = bid_value;
	updateDict();
}

Player Sale::loadPlayerInfos(std::string username, int id){
	Player toFind;
	JSON::Value* loaded = JSON::load(getPlayerPath().c_str());
	if(ISLIST(loaded)){
		JSON::List & playersList = LIST(loaded);
		for(size_t i=0; i<playersList.len();++i){
			if(ISDICT(playersList[i])){
				JSON::Dict & player = DICT(playersList[i]);
				if(INT(player.get(net::MSG::PLAYER_ID)) == id) {
					toFind = DICT(playersList[i]);
				}
			}
		}
	}
	return toFind;
}

void Sale::updateDict(int timer){
	_repr.set(net::MSG::BID_TIMER, timer);
}

void Sale::updateDict(){
	_repr.set(net::MSG::TEAMS_BIDDING, JSON::List());
	for(size_t i=0;i<_turnTeams.size();++i){
		LIST(_repr.get(net::MSG::TEAMS_BIDDING)).append(_turnTeams[i]);
	}
	_repr.set(net::MSG::CAN_BID_TEAMS, JSON::List());
	for(size_t i=0;i<_canBidTeams.size();++i){LIST(_repr.get(net::MSG::CAN_BID_TEAMS)).append(_canBidTeams[i]);}
	_repr.set(net::MSG::BID_VALUE, _bidValue);
	_repr.set(net::MSG::BID_TIMER, _timeLeft);
	_repr.set(net::MSG::TURN_NUMBER, _turn);
	_repr.set(net::MSG::CURRENT_BIDDER, _currentBidder);
}

void Sale::save(){
	updateDict();
	Player toSave = loadPlayerInfos(_owner, _saleID);
	_repr.set(net::MSG::PLAYER, (JSON::Dict)toSave);
	std::cout << getSalePath().c_str() << std::endl;
	_repr.save(getSalePath().c_str());
}




