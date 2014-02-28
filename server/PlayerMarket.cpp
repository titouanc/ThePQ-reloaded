#include "PlayerMarket.hpp"
#include <Constants.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <Exception.hpp>
#include <model/MemoryAccess.hpp>


PlayerMarket::PlayerMarket(Server *server): _server(server), _sales(),
_thread(),_runChecker(true), _deleting(PTHREAD_MUTEX_INITIALIZER) {
	mkdir(memory::MARKET_PATH.c_str(), 0755);
	MemoryAccess::load(_sales);
	for(size_t i=0;i<_sales.size();++i){
		_sales[i]->start();
	}
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
				market->resolveEndOfSale(market->_sales[i]);
				MemoryAccess::removeObject(*(market->_sales[i]));
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

void PlayerMarket::createSale(const JSON::Dict &json){//TODO : Should not access memory
	//json infos
	int player_id = INT(json.get(net::MSG::PLAYER_ID));
	int bidValue = INT(json.get(net::MSG::BID_VALUE));
	std::string username = STR(json.get(net::MSG::USERNAME)).value();
	//create sale, save and start it
	Player player(player_id, username);
	MemoryAccess::load(player);
	_sales.push_back(new Sale(bidValue, username, player_id, player));
	Sale *added = getSale(player_id);
	added->save();
	added->start();
}

void PlayerMarket::transfert(std::string fromName, std::string toName, int id, int bidValue){
	User* from = _server->getUserByName(fromName);
	Player toTransfert;
	if(from == NULL){	//User not connected : load.
		Team fromTeam(fromName);
		fromTeam.load();
		toTransfert = fromTeam.getPlayer(id);
		fromTeam.removePlayer(id);
		fromTeam.getPayed(bidValue);
		fromTeam.save();
	}
	else{				//User connected : use ref.
		Team & fromTeam = from->getTeam();
		toTransfert = fromTeam.getPlayer(id);
		fromTeam.removePlayer(id);
		fromTeam.getPayed(bidValue);
		fromTeam.save();
	}
	toTransfert.setOwner(toName);
	User* to= _server->getUserByName(toName);
	if(to == NULL){
		Team toTeam(toName);
		toTeam.load();
		toTeam.addPlayer(toTransfert);
		toTeam.buy(bidValue);
		toTeam.save();
	}
	else{
		Team & toTeam = to->getTeam();
		toTeam.addPlayer(toTransfert);
		toTeam.buy(bidValue);
		toTeam.save();
	}

}
void PlayerMarket::resolveEndOfSale(Sale * sale){
	if(sale->getCurrentBidder().empty()){//Player not sold
		JSON::Dict toOwner;
		toOwner.set("type",net::MSG::END_OF_OWNED_SALE_RAPPORT);
		toOwner.set(net::MSG::RAPPORT_SALE_STATUS, net::MSG::PLAYER_NOT_SOLD);
		toOwner.set(net::MSG::PLAYER_ID, sale->getID());
		sendMessageToUser(sale->getOwner(), toOwner);
	}
	else{
		transfert(sale->getOwner(),sale->getCurrentBidder(),sale->getID(),sale->getBidValue());
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
	else if(_server->getUserByName(STR(json.get(net::MSG::USERNAME)).value())->getTeam().getPlayers().size() <= gameconfig::MIN_PLAYERS){
		response.set("data",net::MSG::NOT_ENOUGH_PLAYERS);
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
	if(sale == NULL or sale->isOver())
		response.set("data", net::MSG::BID_ENDED);
	else if(_server->getUserByName(username)->getTeam().getPlayers().size() >= gameconfig::MAX_PLAYERS)
		response.set("data", net::MSG::TOO_MANY_PLAYERS);
	else if(_server->getUserByName(username)->getTeam().getFunds() < bid_value){
		response.set("data",net::MSG::INSUFFICIENT_FUNDS);
	}
	else{
		try{
			sale->placeBid(username, bid_value);
			response.set("data", net::MSG::BID_PLACED);
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
	deletingUnlock();
	return response;
}

void PlayerMarket::sendMessageToUser(std::string username, const JSON::Dict & message){
	_server->sendMarketMessage(username, message);
}
