#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <model/Sale.hpp>
#include "ClientManager.hpp"
#include "Menu.hpp"
#include "Exception.hpp"

class MarketManager : public ClientManager
{
private:
	std::vector<Sale> _sales;
protected:
	/* compute next bid value for a player in the market */
	int getNextBidValue(int player_id);
	pair<int, int> getBidValueRange(Player *player);
	std::vector<Sale> const & getSales() { return _sales; }
	// TODO replace player_id by const Player*
	/* asks for the list of current sales in the market */
	void updateSales();
	/* bid request on a player in the market */
	void bidOnPlayer(int player_id);
	/* attempt to sell a player in the market */
	void addPlayerOnMarket(int player_id, int value);

	/* HOOKS */
	/* received list of current sales in the market */
	virtual void onSalesUpdate(JSON::List const &);
	/* received response from a bid request on a player */
	virtual void onPlayerBid(std::string){}
	/* received response from an attempt to sell a player in the market */
	virtual void onAddPlayerOnMarket(std::string){}
	/* treat Market specific messages */
	void treatMessage(std::string const & type, JSON::Value const* data);
public:
	using ClientManager::ClientManager;
	MarketManager(ClientManager const & parent);
};

#endif // __MARKET_MANAGER_HPP
