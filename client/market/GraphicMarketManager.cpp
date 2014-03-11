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

	_canvas.setBackgroundImage(texturePath("HexBack.png"));

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

void GraphicMarketManager::placeBid(int playerID)
{
	_wait=true;
	bidOnPlayer(playerID);
	while (_wait)
		readMessage();
}

void GraphicMarketManager::onSalesUpdate()
{
	_wait = false;
	_canvas.clear();

	TableView & playerList = _canvas.addTableView(1, 5);
	/* Header line */
	TableCell & header = playerList.addTableCell(800, 60);
	header.addLabel("Player name").setPosition(0, 10);
	header.addLabel("Owner").setPosition(300, 10);
	header.addLabel("Bidder").setPosition(400, 10);
	header.addLabel("Price").setPosition(550, 10);
	header.addLabel("Time left").setPosition(650, 10);

	/* Content */
	for (Sale const & sale : getSales()){
		TableCell & player = playerList.addTableCell(800, 60);
		player.addLabel(sale.getPlayer().getName()).setPosition(0, 10);
		if (sale.getOwner() == net::MSG::GENERATED_BY_MARKET)
			player.addLabel("World").setPosition(300, 10);
		else
			player.addLabel(sale.getOwner()).setPosition(300, 10);
		player.addLabel(sale.getCurrentBidder()).setPosition(400, 10);
		player.addLabel(sale.getNextBidValue()).setPosition(550, 10);
		player.addLabel(sale.getTimeLeft()).setPosition(650, 10);
		player.addButton<GraphicMarketManager, int>(
			&GraphicMarketManager::placeBid, sale.getPlayer().getMemberID(),
			this, "BID"
		).setPosition(700, 0);
	}

	_canvas.addButton<GraphicMarketManager>(
		&GraphicMarketManager::updateSales, this, "Update"
	).setPosition(900, 650);

	backButton().setPosition(1050, 650);

	redrawCanvas();
}

void GraphicMarketManager::onBidOK()
{
	_wait = false;
	updateSales();
}

void GraphicMarketManager::onBidError(std::string const & err)
{
	_wait = false;
	displayError(err);
	updateSales();
}
