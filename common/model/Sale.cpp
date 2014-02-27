#include "Sale.hpp"
#include <Constants.hpp>
#include <Exception.hpp>
#include <model/MemoryAccess.hpp>

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
	while( _canBidTeams.size()>1 or _turn == 1 ){
		for(int i=0; i<getTotalTime(); ++i){
			sleep(1);
			--_timeLeft;
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
	save();
	unlock();
}

Sale::Sale() : _turnTeams(), _canBidTeams(), _bidValue(0),_bidRatio(BIDRATIO),
_turn(1),_currentBidder(""),_owner(""), _marketPath("data/PlayerMarket/"),_playerPath("data/"),
_timeLeft(FIRST_TURN), _saleID(0), _thread(), 
_mymutex(PTHREAD_MUTEX_INITIALIZER), _player(Player()), _ended(false) {}

Sale::Sale(int bidValue, std::string owner, int id, const Player & player) : Sale()
{
	_bidValue = bidValue;
	_owner = owner;
	_saleID = id;
	_player = player;
}

Sale::Sale(JSON::Dict const & json): Sale() 
{
	if(json.hasKey(net::MSG::PLAYER)) 			{_player=Player(DICT(json.get(net::MSG::PLAYER)));}
	if(json.hasKey(net::MSG::PLAYER_ID)) 		{_saleID=INT(json.get(net::MSG::PLAYER_ID));}
	if(json.hasKey(net::MSG::USERNAME)) 		{_owner=STR(json.get(net::MSG::USERNAME)).value();}
	if(json.hasKey(net::MSG::CURRENT_BIDDER))	{_currentBidder=STR(json.get(net::MSG::CURRENT_BIDDER)).value();}
	if(json.hasKey(net::MSG::TURN_NUMBER)) 		{_turn = INT(json.get(net::MSG::TURN_NUMBER));}
	if(json.hasKey(net::MSG::BID_TIMER)) 		{_timeLeft = INT(json.get(net::MSG::BID_TIMER));}
	if(json.hasKey(net::MSG::BID_VALUE)) 		{_bidValue = INT(json.get(net::MSG::BID_VALUE));}
	if(json.hasKey(net::MSG::TEAMS_BIDDING)) {
		JSON::List & bidding = LIST(json.get(net::MSG::TEAMS_BIDDING));
		for(size_t i =0;i<bidding.len();++i){
			if(ISSTR(bidding[i]))
				_turnTeams.push_back(STR(bidding[i]).value()); 
		}
	}
	if(json.hasKey(net::MSG::CAN_BID_TEAMS)) {
		JSON::List & canbid = LIST(json.get(net::MSG::CAN_BID_TEAMS));
		for(size_t i =0;i<canbid.len();++i){
			if(ISSTR(canbid[i]))
				_canBidTeams.push_back(STR(canbid[i]).value()); 
		}
	}
}

Sale::Sale(const JSON::Dict & json, const Player & player): Sale(json) {_player = player;}

Sale::Sale(const Sale & other): _turnTeams(other._turnTeams), _canBidTeams(other._canBidTeams), 
_bidValue(other._bidValue),_bidRatio(other._bidRatio),_turn(other._turn),_currentBidder(other._currentBidder),
_owner(other._owner),_marketPath(other._marketPath),_playerPath(other._playerPath),
_timeLeft(other._timeLeft), _saleID(other._saleID), _thread(), 
_mymutex(PTHREAD_MUTEX_INITIALIZER), _player(other._player), _ended(other._ended)
{}



//Test functions
bool Sale::isSaler(std::string username) const{
	return (username == getOwner());
}

bool Sale::allowedToBidForThisTurn(std::string username) const{
	if(_turn == 1){return true;}
	else{
		for(size_t i=0;i<_canBidTeams.size();++i){
			if(_canBidTeams[i] == username){return true;}
		}
	}
	return false;
}

//Services
void Sale::placeBid(std::string username, int bid_value){
	lock();
	if(isSaler(username)) 
		throw bidOnYourPlayerException();
	else if(!(allowedToBidForThisTurn(username)))
		throw turnException();
	else if(getCurrentBidder() == username)
		throw lastBidderException();
	else if(getNextBidValue() != bid_value)
		throw bidValueNotUpdatedException();
	_currentBidder = username;
	_turnTeams.push_back(username);
	_bidValue = bid_value;
	unlock();
}

void Sale::save(){
	MemoryAccess::save(*this);
}

void Sale::load(){
	MemoryAccess::load(*this);
}

Sale::operator JSON::Dict(){
	JSON::Dict ret;
	JSON::List bidding, canBid;
	for(size_t i=0;i<_turnTeams.size();++i){
		bidding.append(_turnTeams[i]);
	}
	for(size_t i=0;i<_canBidTeams.size();++i){
		canBid.append(_canBidTeams[i]);
	}
	ret.set(net::MSG::TEAMS_BIDDING, bidding);
	ret.set(net::MSG::CAN_BID_TEAMS, canBid);
	ret.set(net::MSG::BID_VALUE, _bidValue);
	ret.set(net::MSG::BID_TIMER, _timeLeft);
	ret.set(net::MSG::TURN_NUMBER, _turn);
	ret.set(net::MSG::CURRENT_BIDDER, _currentBidder);
	ret.set(net::MSG::USERNAME,_owner);
	ret.set(net::MSG::PLAYER_ID,_saleID);
	ret.set(net::MSG::PLAYER,JSON::Dict(_player)); 
	return ret;
}

std::ostream& operator<< (std::ostream& out, const Sale& sale){
	out << "PLAYER INFOS : " << std::endl;
	out << sale._player << std::endl;
	out << "SALE INFOS :\n---------------------------------------------------" 		<< std::endl;
	out << "Time left (in seconds) :    " 	<< 	sale._timeLeft 			<< std::endl;
	out << "Bid value :                 "	<<	sale._bidValue 			<< std::endl;
	out << "Next bid value :            "	<< 	sale.getNextBidValue()	<< std::endl;
	std::string bidder = sale._currentBidder;
	if(bidder.empty()){bidder = "No one has bid on this player";}
	out << "Current bidder :            "	<<	bidder 					<< std::endl;
	out << "Sale ID :                   "	<<	sale._saleID			<< std::endl;
	out << "**************************************************" << std::endl;
    return out;
}
