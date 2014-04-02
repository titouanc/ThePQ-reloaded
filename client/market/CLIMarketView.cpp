#include "CLIMarketView.hpp"
#include <match/CLIMatchManager.hpp>

///Constructor
CLIMarketView::CLIMarketView(ClientController const & parent) :
MarketController(parent), _waitForSales(false), _waitForBid(false)
{}

/**
 * Method starting the player market for the command line
 */
void CLIMarketView::run()
{
	while (user().players.size() == 0){
		readMessage();
	}
	Menu _menu;
	_menu.addToDisplay("   - put a player on sale\n");
	_menu.addToDisplay("   - see the players on sale\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	_pending = 0;
	do {
		readMessages();
		option = _menu.run();
		switch(option)
		{
			case 1:
				sellPlayer();
				break;
			case 2:
				showBidMenu();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

/**
 * Method handling the sell of a player
 */
void CLIMarketView::sellPlayer()
{
	showPlayers();
	int player_id, bidValue;
	bool found = false;
	Player * player;
	cout << "Choose a player to sale by entering his ID :" <<endl;
	prompt();
	cin >> player_id;
	for(size_t i = 0; i<user().players.size(); ++i){
		if(user().players[i].getMemberID() == player_id){
			player = &(user().players[i]);
			found = true;
		}
	}
	if(found){
		pair<int, int> range = getBidValueRange(player);
		cout << "Enter the starting bid value (must be between " << range.first << " and " << range.second << ") :" << endl;
		prompt();
		cin >> bidValue;
		while(bidValue<range.first or bidValue>range.second){
			cout << bidValue << " is not between " << range.first << " and " << range.second << " !\nTry again :" << endl;//modif
			prompt();
			cin >> bidValue;
		}
		_waitForBid = true;
		addPlayerOnMarket(player_id, bidValue);
		while (_waitForBid)
			readMessage();
	}
	else{
		cout << "Wrong ID." << endl;
	}
}

/**
 * Method handling the listing of players for sale
 */
void CLIMarketView::displayPlayersOnSale()
{
	_waitForSales = true;
	updateSales();
	while (_waitForSales)
		readMessage();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<getSales().size();++i){
		std::cout<<getSales()[i]<<std::endl;
	}
	cout << "=================================================" << endl;
}

/**
 * Method showing the bidding interface
 */
void CLIMarketView::showBidMenu()
{
	displayPlayersOnSale();
	Menu _menu;
	_menu.addToDisplay("   - place a bid on a player\n");
	_menu.addToDisplay("   - update list\n");
	_menu.addToDisplay("   - quit to market menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				placeBid();
				break;
			case 2:
				displayPlayersOnSale();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

/**
 * Method displaying current users players
 */
void CLIMarketView::showPlayers(){
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

/**
 * Method handling bidding
 */
void CLIMarketView::placeBid()
{
	int player_id;
	string response;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	prompt();
	cin >> player_id;
	try{
		_waitForBid = true;
		bidOnPlayer(player_id);
		while (_waitForBid)
			readMessage();
	}
	catch(PlayerNotFoundException& e) {
		cout << "\033[1;31mError\033[0m : the player id you entered is not correct" << endl;
	}
}

/**
 * Method handling a correct bid
 *(send query to server)
 */
void CLIMarketView::onBidOK()
{
	_waitForBid = false;
	okMsg("Bid added");
}

/**
 * Method handling an incorrect bid
 *(send query to server)
 * @param string: error message
 */
void CLIMarketView::onBidError(std::string const & reason)
{
	_waitForBid = false;
	errorMsg(reason);
}

/**
 * Method handling the addition of a player on the PlayerMarket
 */
void CLIMarketView::onAddPlayerOK()
{
	_waitForBid = false;
	okMsg("Player put on market");
}

/**
 * Method handling an error in the addition of a player on the PlayerMarket
 * @param : string  error message to be sent to the server
 */
void CLIMarketView::onAddPlayerError(std::string const & reason)
{
	_waitForBid = false;
	errorMsg(reason);
}

/**
 * Method handling the update of the market
 */
void CLIMarketView::onSalesUpdate()
{
	_waitForSales = false;
}

/**
 * Method handling the start of a match
 */
void CLIMarketView::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}
