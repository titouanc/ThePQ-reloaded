#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <model/Sale.hpp>
#include "ClientManager.hpp"
#include "Menu.hpp"
#include "Exception.hpp"

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

protected:
	virtual void onSalesUpdate(JSON::List const &);
	virtual void onPlayerBid(std::string){}
	virtual void onAddPlayerOnMarket(std::string){}
	void treatMessage(std::string const & type, JSON::Value const* data);

private:
	int getNextBidValue(int player_id);
	
	std::vector<Sale> _sales;
};

#endif // __MARKET_MANAGER_HPP
