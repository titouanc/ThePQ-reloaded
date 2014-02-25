#include "PlayerMarket.hpp"
#include <Constants.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <Exception.hpp>


PlayerMarket::PlayerMarket(Server *server): _server(server), _sales(), _marketPath("data/playerMarket/"), _playerPath("data/"),
_thread(),_runChecker(true), _deleting(PTHREAD_MUTEX_INITIALIZER) {
	mkdir(_marketPath.c_str(), 0755);
	//loadSales();
	startChecker();
}

PlayerMarket::~PlayerMarket(){
	_runChecker = false;
	for(size_t i=0;i<_sales.size();++i){
		delete _sales[i];
	}
}

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
		throw "Error occured when creating checker thread";
	}
}

void PlayerMarket::createSale(const JSON::Dict &json){
	//json infos
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bidValue = INT(json.get(net::MSG::BID_VALUE));
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	//create sale, save and start it
	Player player;
	player.load(player_id);
	_sales.push_back(new Sale(bidValue, username, player_id, player));
	Sale *added = getSale(player_id);
	added->save();
	added->start();
}

std::vector<Player> getPlayers(std::string username){
	Player util;
	std::vector<Player> ret;
	JSON::Value* loaded = JSON::load(util.getPlayersPath(username));
	JSON::List & players = *((JSON::List*)loaded);
	for(size_t i = 0; i<players.len();++i){
		ret.push_back(DICT(players[i]));
	}
	delete loaded;
	return ret;
}

void removePlayer()

void addPlayer()


void PlayerMarket::transfert(Sale * sale){//REFACTOR THIS SHIT
	if(sale->getCurrentBidder().empty()){//Player not sold
		JSON::Dict toOwner;
		toOwner.set("type",net::MSG::END_OF_OWNED_SALE_RAPPORT);
		toOwner.set(net::MSG::RAPPORT_SALE_STATUS, net::MSG::PLAYER_NOT_SOLD);
		toOwner.set(net::MSG::PLAYER_ID, sale->getID());
		sendMessageToUser(sale->getOwner(), toOwner);
	}
	else{
		std::vector<Player> from, to;
		JSON::List delUpdated, addUpdated;
		Player *toTransfert;
		JSON::Value* delLoad = JSON::load(getPlayersPath(sale->getOwner()).c_str());
		JSON::Value* addLoad = JSON::load(getPlayersPath(sale->getCurrentBidder()).c_str());
		JSON::List & delTeamPlayers = LIST(delLoad);
		JSON::List & addTeamPlayers = LIST(addLoad);
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
			delUpdated.append(JSON::Dict(from[i]));
		}
		for(size_t i=0;i<to.size();++i){
			addUpdated.append(JSON::Dict(to[i]));
		}
		delUpdated.save(getPlayersPath(sale->getOwner()).c_str());
		addUpdated.save(getPlayersPath(sale->getCurrentBidder()).c_str());
		delete toTransfert;
		JSON::Dict toOwner, toWinner;
		toOwner.set("type",net::MSG::END_OF_OWNED_SALE_RAPPORT);
		toOwner.set(net::MSG::RAPPORT_SALE_STATUS, net::MSG::PLAYER_SOLD);
		toOwner.set(net::MSG::PLAYER_ID, sale->getID());
		toOwner.set(net::MSG::BID_VALUE, sale->getBidValue());
		toOwner.set(net::MSG::CURRENT_BIDDER, sale->getCurrentBidder());
		sendMessageToUser(sale->getOwner(), toOwner);
		toWinner.set("type",net::MSG::WON_SALE_RAPPORT);
		toWinner.set(net::MSG::PLAYER_ID,sale->getID());
		toWinner.set(net::MSG::BID_VALUE, sale->getBidValue());
		toWinner.set(net::MSG::SALE_OWNER, sale->getOwner());
		sendMessageToUser(sale->getCurrentBidder(), toWinner);
	}
}





Sale * PlayerMarket::getSale(int id){
	for(size_t i = 0; i<_sales.size();++i){
		if(_sales[i]->getID() == id){return _sales[i];}
	}
	return NULL;
}

JSON::Dict PlayerMarket::allSales() {
	deletingLock();
	JSON::Dict response;
	response.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::List sales;
	for(size_t i=0;i<_sales.size();++i){
		sales.append(JSON::Dict(*(_sales[i])));
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

JSON::Dict PlayerMarket::bid(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bid_value = INT(json.get(net::MSG::BID_VALUE));
	deletingLock();		//Sale checker cannot delete any sale while the validity of the bid is checked
	Sale * sale = getSale(player_id);
	if(sale != NULL and !(sale->isOver())){
		try{
			sale->placeBid(username, bid_value);
		}
		catch(bidValueNotUpdatedException e){
			response.set("data", net::MSG::BID_VALUE_NOT_UPDATED);
		}
		catch(turnException e){
			response.set("data", net::MSG::BID_TURN_ERROR);
		}
		catch(bidOnYourPlayerException e){
			response.set("data", net::MSG::CANNOT_BID_ON_YOUR_PLAYER);
		}
		catch(lastBidderException e){
			response.set("data", net::MSG::LAST_BIDDER);
		}
	}
	else{
		response.set("data", net::MSG::BID_ENDED);
	}
	deletingUnlock();
	return response;
}

// Player PlayerMarket::loadPlayerInfos(std::string username, int id){
// 	Player toFind;
// 	JSON::Value* loaded = JSON::load(getPlayersPath(username).c_str());
// 	if(ISLIST(loaded)){
// 		JSON::List & playersList = LIST(loaded);
// 		for(size_t i=0; i<playersList.len();++i){
// 			if(ISDICT(playersList[i])){
// 				JSON::Dict & player = DICT(playersList[i]);
// 				if(INT(player.get(net::MSG::PLAYER_ID)) == id) {
// 					toFind = DICT(playersList[i]);
// 				}
// 			}
// 		}
// 	}
// 	return toFind;
// }

void PlayerMarket::sendMessageToUser(std::string username, const JSON::Dict & message){
	_server->sendMarketMessage(username, message);
}
