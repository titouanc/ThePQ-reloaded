#ifndef __MARKET_MANAGER_HPP
#define __MARKET_MANAGER_HPP

#include <vector>
#include <model/Sale.hpp>
#include <ClientController.hpp>
#include <cli/Menu.hpp>
#include <Exception.hpp>

class MarketController : public ClientController
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

	/* treat Market specific messages */
	void treatMessage(std::string const & type, JSON::Value const* data);

	/* HOOKS */
	/* received list of current sales in the market */
	virtual void onSalesUpdate(){}

	/* Successfully placed bid */
	virtual void onBidOK(){}
	/* Failed to place bid */
	virtual void onBidError(std::string const & err){}

	/* Successfully added player on market */
	virtual void onAddPlayerOK(){}
	/* Failed to add player on market */
	virtual void onAddPlayerError(std::string const & err){}
public:
	using ClientController::ClientController;
	MarketController(ClientController const & parent);
};

#endif // __MARKET_MANAGER_HPP
