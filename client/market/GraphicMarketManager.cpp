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
	playerList.setPosition(100, 100);
	/* Header line */
	TableCell & header = playerList.addTableCell(800, BUTTON_HEIGHT);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	Label & nameHeader = header.addLabel("Player name");
	nameHeader.setPosition(15, BUTTON_TOP_PADDING);
	nameHeader.setColor(BUTTON_TEXT_COLOR);

	Label & ownerHeader = header.addLabel("Owner");
	ownerHeader.setPosition(300, BUTTON_TOP_PADDING);
	ownerHeader.setColor(BUTTON_TEXT_COLOR);

	Label & bidderHeader = header.addLabel("Bidder");
	bidderHeader.setPosition(400, BUTTON_TOP_PADDING);
	bidderHeader.setColor(BUTTON_TEXT_COLOR);

	Label & priceHeader = header.addLabel("Price");
	priceHeader.setPosition(550, BUTTON_TOP_PADDING);
	priceHeader.setColor(BUTTON_TEXT_COLOR);

	Label & timeHeader = header.addLabel("Time left");
	timeHeader.setPosition(650, BUTTON_TOP_PADDING);
	timeHeader.setColor(BUTTON_TEXT_COLOR);


	/* Content */
	for (Sale const & sale : getSales()){
		TableCell & player = playerList.addTableCell(800, BUTTON_HEIGHT);
		player.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));

		Label & nameLabel = player.addLabel(sale.getPlayer().getName());
		nameLabel.setPosition(15, BUTTON_TOP_PADDING);
		nameLabel.setColor(BUTTON_TEXT_COLOR);

		if (sale.getOwner() == net::MSG::GENERATED_BY_MARKET){
			Label & ownerLabel = player.addLabel("World");
			ownerLabel.setPosition(300, BUTTON_TOP_PADDING);
			ownerLabel.setColor(BUTTON_TEXT_COLOR);
		}
		else{
			Label & ownerLabel = player.addLabel(sale.getOwner());
			ownerLabel.setPosition(300, BUTTON_TOP_PADDING);
			ownerLabel.setColor(BUTTON_TEXT_COLOR);
		}
		Label & bidderLabel = player.addLabel(sale.getCurrentBidder());
		bidderLabel.setPosition(400, BUTTON_TOP_PADDING);
		bidderLabel.setColor(BUTTON_TEXT_COLOR);

		Label & bidValueLabel = player.addLabel(sale.getNextBidValue());
		bidValueLabel.setPosition(550, BUTTON_TOP_PADDING);
		bidValueLabel.setColor(BUTTON_TEXT_COLOR);

		Label & timeLabel = player.addLabel(sale.getTimeLeft());
		timeLabel.setPosition(650, BUTTON_TOP_PADDING);
		timeLabel.setColor(BUTTON_TEXT_COLOR);

		Button<GraphicMarketManager, int> & bidButton = player.addButton<GraphicMarketManager, int>(
			&GraphicMarketManager::placeBid, sale.getPlayer().getMemberID(),
			this, "BID");
		bidButton.setPosition(800-bidButton.getWidth(), 0);
	}

	_canvas.addButton<GraphicMarketManager>(
		&GraphicMarketManager::updateSales, this, "Update"
	).setPosition(1000, 650);

	backButton().setPosition(1150, 650);

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
