#ifndef GRAPHIC_MARKET_MANAGER_HPP
#define GRAPHIC_MARKET_MANAGER_HPP

#include <graphics/GraphicManager.hpp>
#include "MarketManager.hpp"

class GraphicMarketManager : public MarketManager, public GUI::GraphicManager 
{
public:
	GraphicMarketManager(ClientManager const & parent, GUI::MainController &controller);
	~GraphicMarketManager(){}
private:
	void updateSales();
	void onSalesUpdate();

	void doNothing();
	bool _wait;
};


#endif