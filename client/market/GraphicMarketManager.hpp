#ifndef GRAPHIC_MARKET_MANAGER_HPP
#define GRAPHIC_MARKET_MANAGER_HPP

#include <graphics/GraphicManager.hpp>
#include "MarketManager.hpp"
#include <GUINotifier.hpp>
#include <ctime>

class GraphicMarketManager : public MarketManager, public GUI::GraphicManager 
{
public:
	GraphicMarketManager(ClientController const & parent, GUI::MainController &controller);
	~GraphicMarketManager(){}

	void test(int data){
		std::cout << "DATA: " << data << std::endl;
	}
private:
	void updateSales();
	void placeBid(int playerID);
	void displaySellablePlayers();
	void askPriceForPlayer(Player *player);
	void sellPlayer(Player *player);

	/* HOOKS */
	void onSalesUpdate();
	void onBidOK();
	void onBidError(std::string const & err);
	void onAddPlayerOK();
	void onAddPlayerError(std::string const & reason);

	NOTIFIABLE
	void onLoop();

	bool _wait, _waitForBid;
	time_t _lastUpdated;
	bool _isSellingPlayer;
};


#endif