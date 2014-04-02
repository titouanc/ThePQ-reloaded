
#ifndef __CLI_MARKET_MANAGER_HPP
#define __CLI_MARKET_MANAGER_HPP

#include "MarketController.hpp"
#include <cli/CLI.hpp>

class CLIMarketView : public MarketController, public CLI
{
public:
	using MarketController::MarketController;
	CLIMarketView(ClientController const & parent);
	void run();
private:
	void placeBid();
	void showBidMenu();
	void displayPlayersOnSale();
	void sellPlayer();
	void showPlayers();

	bool _waitForSales, _waitForBid;

	virtual void onSalesUpdate();
	virtual void onBidOK();
	virtual void onBidError(std::string const & err);
	virtual void onAddPlayerOK();
	virtual void onAddPlayerError(std::string const & err);
	virtual void onMatchStart();
};

#endif // __CLI_MARKET_MANAGER_HPP