#include "Sale.hpp"
#include <Constants.hpp>
//TODO : saves
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
	while(_turnTeams.size() > 1 or _turn == 1){
		for(int i=0; i<getTotalTime(); ++i){
			sleep(1);
			--_timeLeft;
			updateDict(_timeLeft);
			save(getSalePath(_saleID));
		}
		resolveEndOfTurn();
	}
	_ended = true;
}
	
void Sale::resolveEndOfTurn(){
	lock();
	++_turn;
	_canBidTeams = _turnTeams;
	_turnTeams.clear();
	_timeLeft = getTotalTime();
	updateDict();
	save(getSalePath(_saleID));
	unlock();
}

Sale::Sale(int bidValue, std::string owner, int id, const Player & player) : 
_turnTeams(), _canBidTeams(), _bidValue(bidValue),_bidRatio(BIDRATIO),
_turn(1),_currentBidder(""),_owner(owner), _marketPath("data/PlayerMarket/"),_playerPath("data/"), _timeLeft(FIRST_TURN), 
_saleID(id), _repr(), _thread(), _mymutex(PTHREAD_MUTEX_INITIALIZER), _player(player), _ended(false)
{
	_repr.set(net::MSG::USERNAME,_owner);
	_repr.set(net::MSG::PLAYER_ID,_saleID);
	_repr.set(net::MSG::PLAYER,JSON::Dict(_player)); 
	//Those values are supposed consts during the sale. That's why they are not updated in updateDict().
}

Sale::Sale(const JSON::Dict & json, const Player & player): 
_turnTeams(), _canBidTeams(), _bidValue(INT(json.get(net::MSG::BID_VALUE))),_bidRatio(BIDRATIO),
_turn(1),_currentBidder(""),_owner(STR(json.get(net::MSG::USERNAME)).value()),
_marketPath("data/PlayerMarket/"),_playerPath("data/"),_timeLeft(FIRST_TURN), 
_saleID(INT(json.get(net::MSG::PLAYER_ID))), _repr(json), _thread(), 
_mymutex(PTHREAD_MUTEX_INITIALIZER), _player(player), _ended(false)
{
	_repr.set(net::MSG::USERNAME,_owner);
	_repr.set(net::MSG::PLAYER_ID,_saleID);
	_repr.set(net::MSG::PLAYER,JSON::Dict(_player));
}

Sale::Sale(const Sale & other): _turnTeams(other._turnTeams), _canBidTeams(other._canBidTeams), 
_bidValue(other._bidValue),_bidRatio(other._bidRatio),_turn(other._turn),_currentBidder(other._currentBidder),
_owner(other._owner),_marketPath(other._marketPath),_playerPath(other._playerPath),
_timeLeft(other._timeLeft), _saleID(other._saleID), _repr(other._repr), _thread(), 
_mymutex(PTHREAD_MUTEX_INITIALIZER), _player(other._player), _ended(other._ended)
{}



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



void Sale::updateDict(int timer){
	_repr.set(net::MSG::BID_TIMER, timer);
}

void Sale::updateDict(){
	JSON::List bidding, canBid;	
	for(size_t i=0;i<_turnTeams.size();++i){
		bidding.append(_turnTeams[i]);
	}
	for(size_t i=0;i<_canBidTeams.size();++i){
		canBid.append(_canBidTeams[i]);
	}
	_repr.set(net::MSG::TEAMS_BIDDING, bidding);
	_repr.set(net::MSG::CAN_BID_TEAMS, canBid);
	_repr.set(net::MSG::BID_VALUE, _bidValue);
	_repr.set(net::MSG::BID_TIMER, _timeLeft);
	_repr.set(net::MSG::TURN_NUMBER, _turn);
	_repr.set(net::MSG::CURRENT_BIDDER, _currentBidder);
}

void Sale::save(std::string path){
	_repr.save(path.c_str());
}

std::ostream& operator<< (std::ostream& out, const Sale& sale){
	out << "---------------------------------------------------" 		<< std::endl;
	out << "PLAYER INFOS				: " << 	sale._player			<< std::endl;
	out << "Time left (in seconds) 		: " << 	sale._timeLeft 			<< std::endl;
	out << "Bid value 					: " <<	sale._bidValue 			<< std::endl;
	out << "Next bid value 				: "	<< 	sale.getNextBidValue()	<< std::endl;
	out << "Sale ID 					: "	<<	sale._saleID			<< std::endl;
    return out;
}
