#ifndef PLAYERMARKET_HPP
#define PLAYERMARKET_HPP 

#include <string>
#include <vector>
#include <json/json.hpp>
#include <model/Sale.hpp>
#include <model/Player.hpp>
#include <pthread.h>
#include "ServerManager.hpp"

class PlayerMarket : public ServerManager {
private:
	std::vector<Sale*> _sales;
	pthread_mutex_t _locker;

	void transfert(std::string,std::string,int,int,Sale* =NULL);
	void createSale(int, int, Player&, std::string);
	void resolveEndOfSale(Sale * sale);
	Sale * getSale(int id);
	/* Used to send notifications (when a sale ends) */
	void sendMessageToUser(std::string, const JSON::Dict&);
	void handleNewBid(std::string,std::string,int,int);
public:
	PlayerMarket(const ServerManager&);
	~PlayerMarket();
	/* Functions called by server, returns what will be send to the user */
	JSON::Dict allSales();
	JSON::Dict addPlayer(const JSON::Dict &json);
	JSON::Dict bidOn(const JSON::Dict &json);
	int winningSales(std::string username);
	int ownedSales(std::string username);
	
    void sendPlayersOnMarketList(int peer_id);
    void addPlayerOnMarket(const JSON::Dict &bid, int peer_id);
    void placeBidOnPlayer(const JSON::Dict &bid, int peer_id);
    void sendMarketMessage(const std::string&, const JSON::Dict&);

    void timeUpdate();
    void timeCreateSale();
	
};

#endif
