#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "User.hpp"
#include "Menu.hpp"
#include "Exception.hpp"
#include <model/Sale.hpp>

class MarketManager
{
public:
	MarketManager(net::ClientConnectionManager& connection, User& user);
	void displayMenu();

private:
	void updateSales();
	void bidOnPlayer(int player_id, std::string username, int value);
	void addPlayerOnMarket(int player_id, std::string username, int value);
	void printPlayersOnSale();
	void salePlayer();
	vector<int> getBidValueRange(Player *player);
	void placeBid();
	void handleEndOfSaleNotification(JSON::Dict&);
	void printPlayers();
	
	net::ClientConnectionManager& _connection;
	User& _user;
	char _prompt;
	std::vector<Sale> _sales;
};

#endif // __MARKET_MANAGER_HPP
