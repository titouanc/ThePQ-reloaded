#include "MarketManager.hpp"

MarketManager::MarketManager(ClientManager const & parent) :
	ClientManager(parent)
{}

pair<int, int> MarketManager::getBidValueRange(Player *player){
	int allowedRangeFromEstimatedValue = 10000; //TODO : in Constants.hpp (should do that for many others variables !)
	pair<int, int> range;
	range.first = (player->estimatedValue() - (int) allowedRangeFromEstimatedValue/2);
	range.second = (player->estimatedValue() + (int) allowedRangeFromEstimatedValue/2);
	return range;
}

void MarketManager::updateSales(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	connection().send(query);
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	
	_sales.clear();
	if (ISLIST(received.get("data"))){
		JSON::List & sales = LIST(received.get("data"));
		for(size_t i = 0; i<sales.len();++i)
			_sales.push_back(Sale(DICT(sales[i]), Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)))));
	}
}

void MarketManager::bidOnPlayer(int player_id){//modif
	int value = getNextBidValue(player_id);
	if (value == -1) throw PlayerNotFoundException();
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,user().username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	query.set("data", data);
	connection().send(query);
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::BID_ON_PLAYER_QUERY);
	JSON::Dict const & received = DICT(serverResponse);
	
	if(ISSTR(received.get("data"))){
		std::string res = STR(received.get("data")).value();
		if(res == net::MSG::BID_VALUE_NOT_UPDATED)
			throw bidValueNotUpdatedException();
		else if(res == net::MSG::BID_TURN_ERROR)
			throw turnException();
		else if(res == net::MSG::BID_ENDED)
			throw bidEndedException();
		else if(res == net::MSG::CANNOT_BID_ON_YOUR_PLAYER)
			throw bidOnYourPlayerException();
		else if(res == net::MSG::LAST_BIDDER)
			throw lastBidderException();
		else if(res == net::MSG::TOO_MANY_PLAYERS)
			throw tooManyPlayersException();
		else if(res == net::MSG::INSUFFICIENT_FUNDS)
			throw insufficientFundsException();
	}
}

void MarketManager::addPlayerOnMarket(int player_id, int value){
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,user().username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	connection().send(query);
	JSON::Value *serverResponse = connection().waitForMsg(net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	JSON::Dict const & received = DICT(serverResponse);
		if(ISSTR(received.get("data"))){
			std::string res = STR((received.get("data"))).value();
			if(res == net::MSG::PLAYER_ALREADY_ON_MARKET)
				throw playerAlreadyOnMarketException();
			else if(res == net::MSG::NOT_ENOUGH_PLAYERS)
				throw notEnoughPlayersException();
		}
}

int MarketManager::getNextBidValue(int player_id)
{
	int value = -1;
	for(size_t i = 0; i<_sales.size();++i){		//Getting the next bid value (which is in the JSON::Dict sent by server)
		if(player_id == _sales[i].getID()){
			value = _sales[i].getNextBidValue();
		}
	}
	return value;
}
