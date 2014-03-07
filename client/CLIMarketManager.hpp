#ifndef __CLI_MARKET_MANAGER_HPP
#define __CLI_MARKET_MANAGER_HPP

#include "MarketManager.hpp"
#include "CLI.hpp"

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
};

#endif // __CLI_MARKET_MANAGER_HPP