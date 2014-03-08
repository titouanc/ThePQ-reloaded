
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

	virtual void onSalesUpdate(JSON::List const &);
	virtual void onPlayerBid(std::string data);
	virtual void onAddPlayerOnMarket(std::string data);
};

#endif // __CLI_MARKET_MANAGER_HPP