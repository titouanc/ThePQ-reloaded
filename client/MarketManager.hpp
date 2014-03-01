#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include "Menu.hpp"
#include "Exception.hpp"
#include <model/Sale.hpp>
#include "TeamManager.hpp"

class MarketManager
{
public:
	MarketManager(net::ClientConnectionManager& connection, UserData& user, TeamManager& teamManager);
	void showMenu();

private:
	void updateSales();
	void bidOnPlayer(int player_id, std::string username, int value);
	void addPlayerOnMarket(int player_id, std::string username, int value);
	void printPlayersOnSale();
	void salePlayer();
	vector<int> getBidValueRange(Player *player);
	void placeBid();
	void handleEndOfSaleNotification(JSON::Dict&);
	
	net::ClientConnectionManager& _connection;
	UserData& _user;
	TeamManager& _teamManager;
	char _prompt;
	std::vector<Sale> _sales;
};

#endif // __MARKET_MANAGER_HPP
