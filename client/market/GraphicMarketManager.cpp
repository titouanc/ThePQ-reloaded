#include "GraphicMarketManager.hpp"
#include <Constants.hpp>

using namespace std;
using namespace GUI;

GraphicMarketManager::GraphicMarketManager(ClientManager const & parent, GUI::MainController &controller) : 
	MarketManager(parent), 
	GraphicManager(controller),
	_wait(false)
{
	displayCanvas();
	updateSales();
}

void GraphicMarketManager::updateSales()
{
	MarketManager::updateSales();
	_wait = true;
	while (_wait){
		readEvent();
		readMessages();
	}
}

void GraphicMarketManager::onSalesUpdate()
{
	_wait = false;
	_canvas.clear();

	TableView & playerList = _canvas.addTableView(6, 0);
	/* Header line */
	playerList.addTableCell(300, 50).addLabel("Player name");
	playerList.addTableCell(100, 50).addLabel("Owner");
	playerList.addTableCell(150, 50).addLabel("Bidder");
	playerList.addTableCell(100, 50).addLabel("Price");
	playerList.addTableCell(100, 50).addLabel("Time left");
	playerList.addTableCell(100, 50);

	/* Content */
	for (Sale const & sale : getSales()){
		playerList.addTableCell().addLabel(sale.getPlayer().getName());
		if (sale.getOwner() == net::MSG::GENERATED_BY_MARKET)
			playerList.addTableCell().addLabel("World");
		else
			playerList.addTableCell().addLabel(sale.getOwner());
		playerList.addTableCell().addLabel(sale.getCurrentBidder());
		playerList.addTableCell().addLabel(sale.getNextBidValue());
		playerList.addTableCell().addLabel(sale.getTimeLeft());
		playerList.addTableCell().addLabel("BID");
	}

	_canvas.addButton<GraphicMarketManager>(
		&GraphicMarketManager::updateSales, this, "Update"
	).setPosition(900, 650);

	backButton().setPosition(1050, 650);

	redrawCanvas();
}
