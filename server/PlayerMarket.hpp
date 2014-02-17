#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include <stdio.h>

#include "Sale.hpp"

#define MARKET_DATA_DIR "data/playerMarket/"

class PlayerMarket{
private:
	std::vector<Sale> _sales;
public:
	PlayerMarket(){}
	bool exists(int player_id){
		for(int i = 0; i<_sales.size();++i){
			if(_sales[i].getID() == player_id){return true;}
		}
		return false;
	}

	JSON::Dict allSales(){
		for(i=0;i<_sales.size();++i){
			
		}
	}

	JSON::Dict addPlayer(const JSON::Dict &json){
		int player_id = INT(json.get(net::MSG::PLAYER_ID).value());
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
		if(exists(player_id)){response.set("data",net::MSG::PLAYER_ON_MARKET);}
		else{
			std::string fileName = MARKET_DATA_DIR + "sale_" + player_id + ".json";
			_sales.push_back(Sale(json));
			_sales[_sales.size()-1].save();
			_sales[_sales.size()-1].start();
			response.set("data", net::MSG::PLAYER_ADDED_ON_MARKET);
		}
		return response;
	}

	JSON::Dict deletePlayer(const JSON::Dict &json){
		int player_id = INT(json.get(net::MSG::PLAYER_ID).value());
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::DELETE_PLAYER_ON_MARKET_QUERY);
		if(exists(player_id)){
			response.set("data",net::MSG::PLAYER_DELETED_OF_MARKET);
			std::string fileName = MARKET_DATA_DIR + "sale_" + player_id + ".json";
			remove(fileName);	//delete the file in memory
			for(i=0;i<_sales.size();++i){
				if (_sales[i].getID() == player_id){_sales.erase(_sales.begin()+i);}
			}	
		}
		else{response.set("data",net::MSG::PLAYER_NOT_ON_MARKET);}
		return response;
	}

	void bid(JSON::Dict *json);


};


#endif