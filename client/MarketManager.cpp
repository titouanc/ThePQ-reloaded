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
	this->say(net::MSG::PLAYERS_ON_MARKET_LIST, JSON::Dict());
}

void MarketManager::bidOnPlayer(int player_id){//modif
	int value = getNextBidValue(player_id);
	if (value == -1) throw PlayerNotFoundException();
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,user().username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	say(net::MSG::BID_ON_PLAYER_QUERY, data);
}

void MarketManager::addPlayerOnMarket(int player_id, int value){
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,user().username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	connection().send(query);
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
	if (type == net::MSG::PLAYERS_ON_MARKET_LIST)
	{
		onSalesUpdate(LIST(data));
	}
	else if (type == net::MSG::BID_ON_PLAYER_QUERY)
	{
		onPlayerBid(STR(data).value());
	}
	else if (type == net::MSG::ADD_PLAYER_ON_MARKET_QUERY)
	{
		onAddPlayerOnMarket(STR(data).value());
	}
}

void MarketManager::onSalesUpdate(JSON::List const & sales)
{
	for(size_t i = 0; i<sales.len();++i){
		Player const & inSale = Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)));
		cout << " +++++ IN SALE: " << inSale.getName() << endl;
		_sales.push_back(Sale(DICT(sales[i]), inSale));
	}
}
