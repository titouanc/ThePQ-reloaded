#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include <stdio.h>

#include "Sale.hpp"
//todo : constructor loading all json files in memory (server crash ?)
class PlayerMarket{
private:
	std::vector<Sale> _sales;
	std::string _marketPath;

	void delete(Sale * sale){
		std::string fileName = getSalePath(sale->getID());
		remove(fileName.c_str());
		for(int i=0;i<_sales.size();++i){
			if (_sales[i].getID() == sale->getID()){_sales.erase(_sales.begin()+i);}
		}
	}

	void createSale(const JSON::Dict &json){
		std::string fileName = getSalePath(INT(json.get(net::MSG::PLAYER_ID)));
		_sales.push_back(Sale(json));
		getSale(INT(json.get(net::MSG::PLAYER_ID)))->start(); //Could use _sales[_sales.size()-1]
	}

public:
	PlayerMarket(): _sales(), _marketPath("data/playerMarket/") {}
	std::string getSalePath(int id){return (_marketPath + "sale_" + std::to_string(id) + ".json");}
	Sale * getSale(int id){
		for(int i = 0; i<_sales.size();++i){
			if(_sales[i].getID() == id){return &_sales[i];}
		}
		return NULL;
	}

	JSON::Dict allSales(){
		JSON::Dict response;
		response.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
		response.set("data", JSON::List());
		JSON::List & sales = LIST(response.get("data"));
		for(int i=0;i<_sales.size();++i){
			sales.append(JSON::Dict());
			DICT(sales[i]) = *(_sales[i].getDict());
		}
		return response;
	}

	JSON::Dict addPlayer(const JSON::Dict &json){
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
		if(getSale(INT(json.get(net::MSG::PLAYER_ID))) != NULL){response.set("data",net::MSG::PLAYER_ALREADY_ON_MARKET);}
		else{
			createSale(json);
			response.set("data", net::MSG::PLAYER_ADDED_ON_MARKET);
		}
		return response;
	}

	JSON::Dict deletePlayer(const JSON::Dict &json){
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::DELETE_PLAYER_ON_MARKET_QUERY);
		Sale * sale = getSale(INT(json.get(net::MSG::PLAYER_ID)));
		if(sale != NULL){
			if(sale->isSaler(INT(json.get(net::MSG::TEAM_ID)))) {
				delete(sale);
				response.set("data",net::MSG::PLAYER_DELETED_OF_MARKET);
			} else{response.set("data",net::MSG::NOT_YOUR_SALE);}
		} else{response.set("data",net::MSG::PLAYER_NOT_ON_MARKET);}
		return response;
	}


	JSON::Dict bid(JSON::Dict &json){
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::BID_ON_PLAYER_QUERY);
		int team_id = INT(json.get(net::MSG::TEAM_ID));
		int player_id = INT(json.get(net::MSG::PLAYER_ID));
		int bid_value = INT(json.get(NET::BID_VALUE));
		Sale * sale = getSale(player_id);
		if(sale != NULL){
			if(sale->isSaler(team_id)) {response.set("data", net::MSG::CANNOT_BID_ON_YOUR_PLAYER);}
			else{
				if(!(sale->canBid(team_id))) {response.set("data", net::MSG::BID_TURN_ERROR);}
				else{
					if(sale->currentBidder() == team_id){response.set("data", net::MSG::LAST_BIDDER);}
					else{
						if(sale->getNextBidValue() != bid_value){response.set("data", net::MSG::BID_VALUE_NOT_UPDATED);}
						else{
							sale->placeBid(team_id, bid_value);
							response.set("data", net::MSG::BID_PLACED);
						}
					}
				}
			}
		} else{response.set("data", net::MSG::BID_ENDED);}
		return response;
	}

};

#endif