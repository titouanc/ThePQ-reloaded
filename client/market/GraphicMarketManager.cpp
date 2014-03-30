#include "GraphicMarketManager.hpp"
#include <Constants.hpp>
#include <stdlib.h> // stoi

using namespace std;
using namespace GUI;

///Constructor
GraphicMarketManager::GraphicMarketManager(ClientManager const & parent, GUI::MainController &controller) : 
	MarketManager(parent), 
	GraphicManager(controller),
	_wait(false),
	_lastUpdated(0)
{
	displayCanvas();

	_canvas.setBackgroundImage("HexBack.png");

	updateSales();
}

/**
 * Method handling the market update
 */
void GraphicMarketManager::updateSales()
{
	MarketManager::updateSales();
	_wait = true;
	while (_wait){
		readEvent();
		readMessages();
	}
}

/**
 * Method handling bidding on a player
 * @param int: id of the player to bid on
 */
void GraphicMarketManager::placeBid(int playerID)
{
	_wait=true;
	bidOnPlayer(playerID);
	while (_wait)
		readMessage();
}

/**
 * Method displaying the players available foor sale
 */
void GraphicMarketManager::displaySellablePlayers()
{
	clear();

	addTopBar(user());

	TableView & playerList = _canvas.addTableView(1, 5);
	playerList.setPosition(100, 100);

	/* Header line */
	TableCell & header = playerList.addTableCell(600, BUTTON_HEIGHT);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	Label & nameHeader = header.addLabel("Player name");
	nameHeader.setPosition(15, BUTTON_TOP_PADDING);
	nameHeader.setColor(BUTTON_TEXT_COLOR);

	Label & minPriceHeader = header.addLabel("Min price");
	minPriceHeader.setPosition(300, BUTTON_TOP_PADDING);
	minPriceHeader.setColor(BUTTON_TEXT_COLOR);

	Label & maxPriceHeader = header.addLabel("Max price");
	maxPriceHeader.setPosition(400, BUTTON_TOP_PADDING);
	maxPriceHeader.setColor(BUTTON_TEXT_COLOR);

	for(size_t i =0; i<user().players.size();++i){
		TableCell & playerCell = playerList.addTableCell(600, BUTTON_HEIGHT);
		playerCell.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));

		Player * player = &user().players[i];
		pair<int, int> range = getBidValueRange(player);

		Label & nameLabel = playerCell.addLabel(player->getName());
		nameLabel.setPosition(15, BUTTON_TOP_PADDING);
		nameLabel.setColor(BUTTON_TEXT_COLOR);

		Label & minPriceLabel = playerCell.addLabel(range.first);
		minPriceLabel.setPosition(300, BUTTON_TOP_PADDING);
		minPriceLabel.setColor(BUTTON_TEXT_COLOR);

		Label & maxPriceLabel = playerCell.addLabel(range.second);
		maxPriceLabel.setPosition(400, BUTTON_TOP_PADDING);
		maxPriceLabel.setColor(BUTTON_TEXT_COLOR);
		
		Button<GraphicMarketManager, Player*> & sellButton = playerCell.addButton<GraphicMarketManager, Player*>(
			&GraphicMarketManager::askPriceForPlayer, player, this, "Sell");
		sellButton.setPosition(600-sellButton.getWidth(), 0);
		
	}

	addBackButton();

	redrawCanvas();
}

/**
 * Method handling the bidding on a player
 * @param Player: player to bid on
 */
void GraphicMarketManager::askPriceForPlayer(Player *player){
	Textbox & priceTextbox = _canvas.addTextbox("Enter a price");
	priceTextbox.setPosition(800, 350);

	_canvas.addButton<GraphicMarketManager, Player*>(
		&GraphicMarketManager::sellPlayer, player, this, "Confirm"
	).setPosition(800, 400);

	redrawCanvas();
}

/**
 * Method handling the sale of a player on the PlayerMarket
 * @param Player : player to sell 
 */
void GraphicMarketManager::sellPlayer(Player *player){
	pair<int, int> range = getBidValueRange(player);
	std::string const & text = _canvas.textboxWithID("Enter a price").getText();

	const char *str = text.c_str();
	char *endptr = NULL;
	int bidValue = strtol(str, &endptr, 10);
	
	if (endptr == str || endptr == NULL)
		displayError(text + " is not a valid price !");
	else if (bidValue<range.first or bidValue>range.second)
		displayError("Please enter a price between the price range!");
	else{
		_wait = true;
		addPlayerOnMarket(player->getMemberID(), bidValue);
		while (_wait)
			readMessage();
		// onSalesUpdate will redraw the canvas
	}
}

/**
 * Method handling the update of the Marketplace
 */
void GraphicMarketManager::onSalesUpdate()
{
	_wait = false;
	_lastUpdated = time(NULL);
	clear();

	addTopBar(user());
	int cellSize = 1100;
	TableView & playerList = _canvas.addTableView(1, 5);
	playerList.setPosition((window().getSize().x - cellSize)/2, 100);
	/* Header line */
	TableCell & header = playerList.addTableCell(cellSize, BUTTON_HEIGHT);
	header.setBackgroundColor(BUTTON_BACKGROUND_COLOR);

	Label & nameHeader = header.addLabel("Player name");
	nameHeader.setPosition(15, BUTTON_TOP_PADDING);
	nameHeader.setColor(BUTTON_TEXT_COLOR);

	Label & strengthHeader = header.addLabel("Strength");
	strengthHeader.setPosition(200,BUTTON_TOP_PADDING);
	strengthHeader.setColor(BUTTON_TEXT_COLOR);

	Label & velocityHeader = header.addLabel("Velocity");
	velocityHeader.setPosition(300,BUTTON_TOP_PADDING);
	velocityHeader.setColor(BUTTON_TEXT_COLOR);

	Label & precisionHeader = header.addLabel("Precision");
	precisionHeader.setPosition(400,BUTTON_TOP_PADDING);
	precisionHeader.setColor(BUTTON_TEXT_COLOR);

	Label & chanceHeader = header.addLabel("Chance");
	chanceHeader.setPosition(500,BUTTON_TOP_PADDING);
	chanceHeader.setColor(BUTTON_TEXT_COLOR);

	Label & ownerHeader = header.addLabel("Owner");
	ownerHeader.setPosition(600, BUTTON_TOP_PADDING);
	ownerHeader.setColor(BUTTON_TEXT_COLOR);

	Label & bidderHeader = header.addLabel("Bidder");
	bidderHeader.setPosition(700, BUTTON_TOP_PADDING);
	bidderHeader.setColor(BUTTON_TEXT_COLOR);

	Label & priceHeader = header.addLabel("Next Price");
	priceHeader.setPosition(800, BUTTON_TOP_PADDING);
	priceHeader.setColor(BUTTON_TEXT_COLOR);

	Label & timeHeader = header.addLabel("Time left");
	timeHeader.setPosition(900, BUTTON_TOP_PADDING);
	timeHeader.setColor(BUTTON_TEXT_COLOR);


	/* Content */
	for (Sale const & sale : getSales()){
		TableCell & player = playerList.addTableCell(cellSize, BUTTON_HEIGHT);
		player.setBackgroundColor(sf::Color(0x00, 0x00, 0x00, 0x77));

		Label & nameLabel = player.addLabel(sale.getPlayer().getName());
		nameLabel.setPosition(15, BUTTON_TOP_PADDING);
		nameLabel.setColor(BUTTON_TEXT_COLOR);

		Label & strengthLabel = player.addLabel(sale.getPlayer().getStrength());
		strengthLabel.setPosition(200,BUTTON_TOP_PADDING);
		strengthLabel.setColor(BUTTON_TEXT_COLOR);

		Label & velocityLabel = player.addLabel(sale.getPlayer().getVelocity());
		velocityLabel.setPosition(300,BUTTON_TOP_PADDING);
		velocityLabel.setColor(BUTTON_TEXT_COLOR);

		Label & precisionLabel = player.addLabel(sale.getPlayer().getPrecision());
		precisionLabel.setPosition(400,BUTTON_TOP_PADDING);
		precisionLabel.setColor(BUTTON_TEXT_COLOR);

		Label & chanceLabel = player.addLabel(sale.getPlayer().getChance());
		chanceLabel.setPosition(500,BUTTON_TOP_PADDING);
		chanceLabel.setColor(BUTTON_TEXT_COLOR);

		if (sale.getOwner() == net::MSG::GENERATED_BY_MARKET){
			Label & ownerLabel = player.addLabel("World");
			ownerLabel.setPosition(600, BUTTON_TOP_PADDING);
			ownerLabel.setColor(BUTTON_TEXT_COLOR);
		}
		else{
			Label & ownerLabel = player.addLabel(sale.getOwner());
			ownerLabel.setPosition(600, BUTTON_TOP_PADDING);
			ownerLabel.setColor(BUTTON_TEXT_COLOR);
		}
		Label & bidderLabel = player.addLabel(sale.getCurrentBidder());
		bidderLabel.setPosition(700, BUTTON_TOP_PADDING);
		bidderLabel.setColor(BUTTON_TEXT_COLOR);

		Label & bidValueLabel = player.addLabel(sale.getNextBidValue());
		bidValueLabel.setPosition(800, BUTTON_TOP_PADDING);
		bidValueLabel.setColor(BUTTON_TEXT_COLOR);

		Label & timeLabel = player.addLabel(sale.getTimeLeft());
		timeLabel.setPosition(900, BUTTON_TOP_PADDING);
		timeLabel.setColor(BUTTON_TEXT_COLOR);

		Button<GraphicMarketManager, int> & bidButton = player.addButton<GraphicMarketManager, int>(
			&GraphicMarketManager::placeBid, sale.getPlayer().getMemberID(),
			this, "BID");
		bidButton.setPosition(cellSize-bidButton.getWidth(), 0);
	}
	
	addBackButton();

	Button<GraphicMarketManager> & sellButton = _canvas.addButton<GraphicMarketManager>(&GraphicMarketManager::displaySellablePlayers, this, "Sell a player");
	sellButton.setPosition(window().getSize().x-_backButton->getWidth()-sellButton.getWidth() - 3*MARGIN, window().getSize().y-sellButton.getHeight()-MARGIN);


	

	redrawCanvas();
}
/**
 * Method handling a correct bid
 */
void GraphicMarketManager::onBidOK()
{
	_wait = false;
	updateSales();
}

/**
 * Method handling an incorrect bid
 * @param string : error message 
 */
void GraphicMarketManager::onBidError(std::string const & err)
{
	_wait = false;
	displayError(err);
	updateSales();
}

/**
 * Method handling correct addition of player on the market
 */
void GraphicMarketManager::onAddPlayerOK()
{
	updateSales();
	while (_wait)
		readMessage();
}

/**
 * Method handling error on addition of player on the market
 * @param string: error message
 */
void GraphicMarketManager::onAddPlayerError(std::string const & reason)
{
	_wait = false;
	displayError(reason);
}

void GraphicMarketManager::onLoop(){
	if (abs(difftime(time(NULL), _lastUpdated)) > GUI::AUTOUPDATE_STALE_TIME)
		updateSales();
}

