#include "PlayerMarket.hpp"
#include <Constants.hpp>
#include <sys/stat.h>
#include <stdio.h>

void * saleChecker(void * p){
	PlayerMarket *market = static_cast<PlayerMarket*>(p);
	while(market->_runChecker){
		sleep(5); //Checks every 5 seconds
		for(size_t i = 0; i<market->_sales.size();++i){
			if(market->_sales[i]->isOver()){
				market->deletingLock();
				market->transfert(market->_sales[i]);
				remove(market->_sales[i]->getSalePath(market->_sales[i]->getID()).c_str());
				delete market->_sales[i];
				market->_sales.erase(market->_sales.begin()+i);
				market->deletingUnlock();
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
		//Player not sold, should send notification to client.
	}
	else{
		std::vector<Player> from, to;
		JSON::List delUpdated, addUpdated;
		Player *toTransfert;
		JSON::Value* delLoad = JSON::load(getPlayersPath(sale->getOwner()).c_str());
		JSON::Value* addLoad = JSON::load(getPlayersPath(sale->getCurrentBidder()).c_str());
		JSON::List & delTeamPlayers = *((JSON::List*)delLoad);
		JSON::List & addTeamPlayers = *((JSON::List*)addLoad);
		for(size_t i=0;i<delTeamPlayers.len();++i){
			if(INT(DICT(delTeamPlayers[i]).get(net::MSG::PLAYER_ID))==sale->getID()){
				toTransfert = new Player(DICT(delTeamPlayers[i]));
			}
			else{
				from.push_back(Player(DICT(delTeamPlayers[i])));
			}
		}
		for(size_t i=0;i<addTeamPlayers.len();++i){
			to.push_back(Player(DICT(addTeamPlayers[i])));
		}
		to.push_back(*toTransfert);
		for(size_t i=0;i<from.size();++i){
			delUpdated.append(from[i]);
		}
		for(size_t i=0;i<to.size();++i){
			addUpdated.append(to[i]);
		}
		delUpdated.save(getPlayersPath(sale->getOwner()).c_str());
		addUpdated.save(getPlayersPath(sale->getCurrentBidder()).c_str());
		delete toTransfert;
	}
}


PlayerMarket::PlayerMarket(): _sales(), _marketPath("data/playerMarket/"), _playerPath("data/"),
_thread(),_runChecker(true), _deleting(PTHREAD_MUTEX_INITIALIZER) {
	mkdir(_marketPath.c_str(), 0755);
	startChecker();
}

Sale * PlayerMarket::getSale(int id){
	for(size_t i = 0; i<_sales.size();++i){
		if(_sales[i]->getID() == id){return _sales[i];}
	}
	return NULL;
}

JSON::Dict PlayerMarket::allSales(){
	deletingLock();
	JSON::Dict response;
	response.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::List sales;
	for(size_t i=0;i<_sales.size();++i){
		sales.append(*(_sales[i]->getDict()));
	}
	response.set("data", sales);
	deletingUnlock();
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
	deletingLock();
	Sale * sale = getSale(player_id);
	sale->lock();
	if(sale != NULL and !(sale->isOver())) {
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
		
	} else{response.set("data", net::MSG::BID_ENDED);}
	sale->unlock();	
	deletingUnlock();
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