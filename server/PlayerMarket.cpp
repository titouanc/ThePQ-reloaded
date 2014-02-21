#include "PlayerMarket.hpp"
#include <Constants.hpp>

void * saleChecker(void * p){
	PlayerMarket *market = static_cast<PlayerMarket*>(p);
	while(market->_runChecker){
		for(size_t i = 0; i<market->_sales.size();++i){
			if(market->_sales[i]->isOver()){
				market->transfert(market->_sales[i]);
				delete market->_sales[i];
				market->_sales.erase(market->_sales.begin()+i);
			}
		}
	}
	return NULL;
}

void PlayerMarket::startChecker(){
	if(pthread_create(&_thread, NULL, saleChecker, this) != 0){
		throw "Error occured when creating sale thread";
	}
}

// void PlayerMarket::removeSale(Sale * sale){//modif
// 	std::string fileName = getSalePath(sale->getID());
// 	remove(fileName.c_str());
// 	delete sale;
// 	for(size_t i=0;i<_sales.size();++i){
// 		if (_sales[i]->getID() == sale->getID()){_sales.erase(_sales.begin()+i);}
// 	}
// }

void PlayerMarket::createSale(const JSON::Dict &json){
	//json infos
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bidValue = INT(json.get(net::MSG::BID_VALUE));
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	//create sale, save and start it
	Player player = loadPlayerInfos (username, player_id);
	_sales.push_back(new Sale(bidValue, username, player_id, player));//modif
	Sale *added = getSale(player_id);
	added->updateDict();
	added->save(added->getSalePath(player_id));
	added->start();
}



void PlayerMarket::transfert(Sale * sale){
	if(sale->_currentBidder.empty()){
		//Player not sold, should send notification to client. (doing nothing is fine too at the moment).
	}
	else{
		//TODO Transfert the player from _owner to _currentBidder (which are usernames)
		//Player playerSold = loadPlayerInfos(_owner, sale->getID());

	}
}


PlayerMarket::PlayerMarket(): _sales(), _marketPath("data/playerMarket/"), _playerPath("data/"),
_thread(),_runChecker(true) {
	startChecker();
}

Sale * PlayerMarket::getSale(int id){
	for(size_t i = 0; i<_sales.size();++i){
		if(_sales[i]->getID() == id){return _sales[i];}
	}
	return NULL;
}

JSON::Dict PlayerMarket::allSales(){
	JSON::Dict response;
	response.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::List sales;
	for(size_t i=0;i<_sales.size();++i){
		sales.append(*(_sales[i]->getDict()));
	}
	response.set("data", sales);
	return response;
}

JSON::Dict PlayerMarket::addPlayer(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	if(getSale(INT(json.get(net::MSG::PLAYER_ID))) != NULL){
		response.set("data",net::MSG::PLAYER_ALREADY_ON_MARKET);
	}
	else{
		createSale(json);
		response.set("data", net::MSG::PLAYER_ADDED_ON_MARKET);
	}
	return response;
}

// JSON::Dict PlayerMarket::deletePlayer(const JSON::Dict &json){//modif
// 	JSON::Dict response = JSON::Dict();
// 	response.set("type", net::MSG::DELETE_PLAYER_OF_MARKET_QUERY);
// 	Sale * sale = getSale(INT(json.get(net::MSG::PLAYER_ID)));
// 	sale->lock();
// 	if(sale != NULL){
// 		if(sale->isSaler(INT(json.get(net::MSG::TEAM_ID)))) {
// 			removeSale(sale);
// 			response.set("data",net::MSG::PLAYER_DELETED_OF_MARKET);
// 		} else{response.set("data",net::MSG::NOT_YOUR_SALE);}
// 	} else{response.set("data",net::MSG::PLAYER_NOT_ON_MARKET);}
// 	sale->unlock();
// 	return response;
// }


JSON::Dict PlayerMarket::bid(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bid_value = INT(json.get(net::MSG::BID_VALUE));
	Sale * sale = getSale(player_id);
	
	if(sale != NULL and !(sale->isOver())) {
		sale->lock();
		if(sale->isSaler(username)) {response.set("data", net::MSG::CANNOT_BID_ON_YOUR_PLAYER);}
		else{
			if(!(sale->allowedToBidForThisTurn(username))) {response.set("data", net::MSG::BID_TURN_ERROR);}
			else{
				if(sale->getCurrentBidder() == username){response.set("data", net::MSG::LAST_BIDDER);}
				else{
					if(sale->getNextBidValue() != bid_value){response.set("data", net::MSG::BID_VALUE_NOT_UPDATED);}
					else{
						sale->placeBid(username, bid_value);
						response.set("data", net::MSG::BID_PLACED);
					}
				}
			}
		}
		sale->unlock();	
	} else{response.set("data", net::MSG::BID_ENDED);}
	
	return response;
}

Player PlayerMarket::loadPlayerInfos(std::string username, int id){
	Player toFind;
	JSON::Value* loaded = JSON::load(getPlayersPath(username).c_str());
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