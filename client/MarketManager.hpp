#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <network/ClientConnectionManager.hpp>
#include "UserData.hpp"
#include "Menu.hpp"
#include "Exception.hpp"
#include <model/Sale.hpp>
#include "TeamManager.hpp"
#include <map>

class MarketManager
{
public:
	MarketManager(net::ClientConnectionManager& connection, UserData& user);
	// TODO replace player_id by const Player*
	void updateSales();
	void bidOnPlayer(int player_id);
	void addPlayerOnMarket(int player_id, int value);
	pair<int, int> getBidValueRange(Player *player);
	
	std::vector<Sale> const & getSales() { return _sales; }
	
private:
	int getNextBidValue(int player_id);
	
	net::ClientConnectionManager& _connection;
	UserData& _user;
	std::vector<Sale> _sales;
};

#endif // __MARKET_MANAGER_HPP
