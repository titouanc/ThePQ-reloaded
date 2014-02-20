#include "PlayerMarket.hpp"
#include <Constants.hpp>

void PlayerMarket::removeSale(Sale * sale){//modif
	std::string fileName = getSalePath(sale->getID());
	remove(fileName.c_str());
	delete sale;
	for(size_t i=0;i<_sales.size();++i){
		if (_sales[i]->getID() == sale->getID()){_sales.erase(_sales.begin()+i);}
	}
}

void PlayerMarket::createSale(const JSON::Dict &json){
	std::string fileName = getSalePath(INT(json.get(net::MSG::PLAYER_ID)));
	_sales.push_back(new Sale(json, this));//modif
	getSale(INT(json.get(net::MSG::PLAYER_ID)))->start(); //Could use _sales[_sales.size()-1]
}
void PlayerMarket::transfert(std::string from_user, std::string to_user, int player_id){
	//TODO 
}


PlayerMarket::PlayerMarket(): _sales(), _marketPath("data/playerMarket/") {}

std::string PlayerMarket::getSalePath(int id){
	return (_marketPath + "sale_" + std::to_string(id) + ".json");
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
	response.set("data", JSON::List());
	JSON::List & sales = LIST(response.get("data"));
	for(size_t i=0;i<_sales.size();++i){
		sales.append(JSON::Dict());
		DICT(sales[i]) = *(_sales[i]->getDict());
	}
	return response;
}

JSON::Dict PlayerMarket::addPlayer(const JSON::Dict &json){
	JSON::Dict response = JSON::Dict();
	response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	if(getSale(INT(json.get(net::MSG::PLAYER_ID))) != NULL){response.set("data",net::MSG::PLAYER_ALREADY_ON_MARKET);}
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
	
	if(sale != NULL){
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