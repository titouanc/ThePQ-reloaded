#include "Sale.hpp"
#include <Constants.hpp>
#include <Exception.hpp>
#include <model/MemoryAccess.hpp>
	
/// Method handling the end of a step in the sale process	
void Sale::resolveEndOfTurn(){
	++_turn;
	_canBidTeams = _turnTeams;
	_turnTeams.clear();
	_timeLeft = getTotalTime();
	if(_canBidTeams.size() <= 1){_ended=true;}
	save();
}

/// Copy constructor
Sale::Sale() : _turnTeams(), _canBidTeams(), _bidValue(0),_bidRatio(gameconfig::BID_RATIO),
_turn(1),_currentBidder(""),_owner(""),_timeLeft(gameconfig::SALE_FIRST_TURN_TIME), _saleID(0), 
_player(Player()), _ended(false) {}

/// Constructor
Sale::Sale(int bidValue, std::string owner, int id, const Player & player) : Sale()
{
	_bidValue = bidValue;
	_owner = owner;
	_saleID = id;
	_player = player;
}

/// Copy constructor
Sale::Sale(JSON::Dict const & json): Sale() 
{
	if(json.hasKey(net::MSG::PLAYER)) 			{_player=Player(DICT(json.get(net::MSG::PLAYER)));}
	if(json.hasKey(net::MSG::PLAYER_ID)) 		{_saleID=INT(json.get(net::MSG::PLAYER_ID));}
	if(json.hasKey(net::MSG::USERNAME)) 		{_owner=STR(json.get(net::MSG::USERNAME)).value();}
	if(json.hasKey(net::MSG::CURRENT_BIDDER))	{_currentBidder=STR(json.get(net::MSG::CURRENT_BIDDER)).value();}
	if(json.hasKey(net::MSG::TURN_NUMBER)) 		{_turn = INT(json.get(net::MSG::TURN_NUMBER));}
	if(json.hasKey(net::MSG::BID_TIMER)) 		{_timeLeft = INT(json.get(net::MSG::BID_TIMER));}
	if(json.hasKey(net::MSG::BID_VALUE)) 		{_bidValue = INT(json.get(net::MSG::BID_VALUE));}
	if(json.hasKey(net::MSG::SALE_STATUS))		{_ended = BOOL(json.get(net::MSG::SALE_STATUS));}
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

/// Copy constructor
Sale::Sale(const JSON::Dict & json, const Player & player): Sale(json) {_player = player;}

/// Copy constructor
Sale::Sale(const Sale & other): _turnTeams(other._turnTeams), _canBidTeams(other._canBidTeams), 
_bidValue(other._bidValue),_bidRatio(other._bidRatio),_turn(other._turn),
_currentBidder(other._currentBidder),_owner(other._owner),_timeLeft(other._timeLeft),
 _saleID(other._saleID),_player(other._player),_ended(other._ended)
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
std::pair<std::string, int> Sale::placeBid(std::string username, int bid_value){
	if(isSaler(username)) 
		throw bidOnYourPlayerException();
	else if(!(allowedToBidForThisTurn(username)))
		throw turnException();
	else if(getCurrentBidder() == username)
		throw lastBidderException();
	else if(getNextBidValue() != bid_value)
		throw bidValueNotUpdatedException();
	std::pair<std::string,int> res;	//Pair used to restore the money to its previous bidder
	res.first = _currentBidder;	
	res.second = _bidValue;
	_currentBidder = username;
	_turnTeams.push_back(username);
	_bidValue = bid_value;
	return res;
}
/// Method saving a sale in the server
void Sale::save(){
	MemoryAccess::save(*this);
}
/// Method loading a sale from the server
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
	ret.set(net::MSG::SALE_STATUS, _ended);
	return ret;
}
/// Ostream operator overload
std::ostream& operator<< (std::ostream& out, const Sale& sale){
	out << sale._player;
	out << " Time(\033[32m" 	<< 	sale._timeLeft << "\033[0m) Value(\033[32m" <<	sale._bidValue <<"\033[0m -> \033[32m"
	<<sale.getNextBidValue()<<  "\033[0m) Bidder(\033[32m"<<sale._currentBidder<<"\033[0m)"<<std::endl;
    return out;
}
