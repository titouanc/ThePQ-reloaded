
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
	void showPlayers();

	virtual void onPlayerBid(std::string data);
	virtual void onAddPlayerOnMarket(std::string data);
	virtual void treatMessage(std::string const & type, JSON::Value const * data);
	virtual void say(std::string const & type, JSON::Value const & data);
};

#endif // __CLI_MARKET_MANAGER_HPP