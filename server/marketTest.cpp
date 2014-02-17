//#include "PlayerMarket.hpp"
#include "json/json.hpp"
#include <iostream>



int main(){
	JSON::Dict client;//ce que le client envoie lorsqu'il met un joueur en vente
	client.set("player", JSON::Dict());
	client.set("bidValue","1000");
	DICT(client.get("player")).set("name","Michel Michel");
	DICT(client.get("player")).set("age","20");
	DICT(client.get("player")).set("team_id","1");
	DICT(client.get("player")).set("player_id","2342")
	DICT(client.get("player")).set("aptitudes",JSON::Dict());
	std::cout<<"JSON sale : "<<client<<std::endl;
	return 0;
}