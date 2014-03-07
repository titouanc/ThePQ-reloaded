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
#include "ClientManager.hpp"

class MarketManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	MarketManager(ClientManager const & parent);
	// TODO replace player_id by const Player*
	void updateSales();
	void bidOnPlayer(int player_id);
	void addPlayerOnMarket(int player_id, int value);
	pair<int, int> getBidValueRange(Player *player);
	
	std::vector<Sale> const & getSales() { return _sales; }
	
private:
	int getNextBidValue(int player_id);
	
	std::vector<Sale> _sales;
};

#endif // __MARKET_MANAGER_HPP
