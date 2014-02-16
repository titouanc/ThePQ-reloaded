#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include <stdio.h>

#define MARKET_DATA_DIR "data/playerMarket/"
class PlayerMarket{
private:
	std::vector<Sale> _sales;
public:
	PlayerMarket(){}

	Message addPlayer(JSON::Dict *json, int peer_id){
		int sale_id = INT(json->get(net::MSG::PLAYER_ID).value());
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
		if(exists(sale_id)){response.set("data",net::MSG::PLAYER_ON_MARKET);}
		else{
			std::string fileName = MARKET_DATA_DIR + "sale_" + sale_id + ".json";
			Sale newSale(json);
			newSale.save(fileName.c_str());
			_sales.push_back(newSale);
			response.set("data", net::MSG::PLAYER_ADDED_ON_MARKET);
		}
		Message status(peer_id, response.clone());
		return status;

	}

	Message deletePlayer(JSON::Dict *json, int peer_id){
		int player_id = INT(json->get(net::MSG::PLAYER_ID).value());
		JSON::Dict response = JSON::Dict();
		response.set("type", net::MSG::DELETE_PLAYER_ON_MARKET_QUERY);
		if(exists(player_id)){
			response.set("data",net::MSG::PLAYER_DELETED_OF_MARKET);
			std::string fileName = MARKET_DATA_DIR + "sale_" + player_id + ".json";
			remove(fileName);
			for(std::list<Sale>::iterator it = _sales.begin(); it != _sales.end(); it++){
				if(it->getID() == player_id){_sales.erase(it);}
			}	
		}
		else{response.set("data",net::MSG::PLAYER_NOT_ON_MARKET);}
		Message status(peer_id, response.clone());
		return status;
	}

	void bid(JSON::Dict *json);
	bool exists(int player_id){
		for(int i = 0; i<_sales.size();++i){
			if(_sales[i].getID() == player_id){return true;}
		}
		return false;
	}

};


#endif