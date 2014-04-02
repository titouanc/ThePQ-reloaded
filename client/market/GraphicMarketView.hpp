#ifndef GRAPHIC_MARKET_MANAGER_HPP
#define GRAPHIC_MARKET_MANAGER_HPP

#include <graphics/GraphicManager.hpp>
#include "MarketController.hpp"
#include <GUINotifier.hpp>
#include <ctime>

class GraphicMarketView : public MarketController, public GUI::GraphicManager 
{
public:
	GraphicMarketView(ClientController const & parent, GUI::MainController &controller);
	~GraphicMarketView(){}

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