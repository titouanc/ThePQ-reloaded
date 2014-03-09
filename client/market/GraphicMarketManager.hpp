#ifndef GRAPHIC_MARKET_MANAGER_HPP
#define GRAPHIC_MARKET_MANAGER_HPP

#include <graphics/GraphicManager.hpp>
#include "MarketManager.hpp"

class GraphicMarketManager : public MarketManager, public GUI::GraphicManager 
{
public:
	GraphicMarketManager(ClientManager const & parent, GUI::MainController &controller);
	~GraphicMarketManager(){}

	void test(int data){
		std::cout << "DATA: " << data << std::endl;
	}
private:
	void updateSales();

	/* HOOKS */
	void onSalesUpdate();
	void onBidOK();
	void onBidError(std::string const & err);

	bool _wait;
};


#endif