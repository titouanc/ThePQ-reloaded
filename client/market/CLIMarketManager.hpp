
#ifndef __CLI_MARKET_MANAGER_HPP
#define __CLI_MARKET_MANAGER_HPP

#include "MarketManager.hpp"
#include <cli/CLI.hpp>

class CLIMarketManager : public MarketManager, public CLI
{
public:
	using MarketManager::MarketManager;
	CLIMarketManager(ClientManager const & parent);
	void run();
private:
	void placeBid();
	void showBidMenu();
	void displayPlayersOnSale();
	void sellPlayer();
	void showPlayers();

	bool _waitForSales, _waitForBid;

	virtual void onSalesUpdate(JSON::List const &players);
	virtual void onBidOK();
	virtual void onBidError(std::string const & err);
	virtual void onAddPlayerOK();
	virtual void onAddPlayerError(std::string const & err);
};

#endif // __CLI_MARKET_MANAGER_HPP