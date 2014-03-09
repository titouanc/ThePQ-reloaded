#include "MarketManager.hpp"

MarketManager::MarketManager(ClientManager const & parent) :
	ClientManager(parent)
{
	updateSales();
	loadPlayers();
}

pair<int, int> MarketManager::getBidValueRange(Player *player){
	int allowedRangeFromEstimatedValue = 10000; //TODO : in Constants.hpp (should do that for many others variables !)
	pair<int, int> range;
	range.first = (player->estimatedValue() - (int) allowedRangeFromEstimatedValue/2);
	range.second = (player->estimatedValue() + (int) allowedRangeFromEstimatedValue/2);
	return range;
}

void MarketManager::updateSales(){
	say(net::MSG::PLAYERS_ON_MARKET_LIST, JSON::String(""));
}

void MarketManager::bidOnPlayer(int player_id){//modif
	int value = getNextBidValue(player_id);
	if (value == -1) throw PlayerNotFoundException();
	JSON::Dict data = {
		{ net::MSG::USERNAME, JSON::String(user().username) },
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::BID_VALUE, JSON::Integer(value) }
	};
	say(net::MSG::BID_ON_PLAYER_QUERY, data);
}

void MarketManager::addPlayerOnMarket(int player_id, int value){
	JSON::Dict data = {
		{ net::MSG::USERNAME, JSON::String(user().username) },
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::BID_VALUE, JSON::Integer(value) }
	};
	say (net::MSG::ADD_PLAYER_ON_MARKET_QUERY, data);
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

void MarketManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::PLAYERS_ON_MARKET_LIST){
		JSON::List const & sales = LIST(data);
		_sales.clear();
		for(size_t i = 0; i<sales.len();++i){
			Player const & onSale = Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)));
			_sales.push_back(Sale(DICT(sales[i]), onSale));
		}
		onSalesUpdate();
	}
	else if (type == net::MSG::BID_ON_PLAYER_QUERY){
		std::string const & response = STR(data);
		if (response == net::MSG::BID_VALUE_NOT_UPDATED)
			onBidError("bid value not correct (update your market list).");
		else if (response == net::MSG::BID_TURN_ERROR)
			onBidError("you did not bid last turn.");
		else if (response == net::MSG::BID_ENDED)
			onBidError("player not on market any more (update your market list).");
		else if (response == net::MSG::CANNOT_BID_ON_YOUR_PLAYER)
			onBidError("cannot bid on your player.");
		else if (response == net::MSG::LAST_BIDDER)
			onBidError("you are currently winning this sale. Cannot bid on your own bid.");
		else if (response == net::MSG::TOO_MANY_PLAYERS)
			onBidError("you have too many players to be able to place a bid.");
		else if (response == net::MSG::INSUFFICIENT_FUNDS)
			onBidError("not enough money (GET MORE $$$$$).");
		else
			onBidOK();
	}
	else if (type == net::MSG::ADD_PLAYER_ON_MARKET_QUERY){
		std::string const & response = STR(data);
		if (response == net::MSG::PLAYER_ALREADY_ON_MARKET)
			onAddPlayerError("you are already selling this player.");
		else if (response == net::MSG::NOT_ENOUGH_PLAYERS)
			onAddPlayerError("Not enough players in your team to put a player on sale.");
		else
			onAddPlayerOK();
	}
}
