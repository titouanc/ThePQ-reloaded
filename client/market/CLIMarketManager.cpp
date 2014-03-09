#include "CLIMarketManager.hpp"
#include <match/CLIMatchManager.hpp>
CLIMarketManager::CLIMarketManager(ClientManager const & parent) :
MarketManager(parent), _waitForSales(false), _waitForBid(false)
{}

void CLIMarketManager::run()
{
	cout << "MARKET MANAGER STARTING..." << endl;
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

void CLIMarketManager::sellPlayer()
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

void CLIMarketManager::displayPlayersOnSale()
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

void CLIMarketManager::showBidMenu()
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

void CLIMarketManager::showPlayers(){
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<user().players.size();++i){
		cout << user().players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

void CLIMarketManager::placeBid()
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

void CLIMarketManager::onBidOK()
{
	_waitForBid = false;
	okMsg("Bid added");
}

void CLIMarketManager::onBidError(std::string const & reason)
{
	_waitForBid = false;
	errorMsg(reason);
}

void CLIMarketManager::onAddPlayerOK()
{
	_waitForBid = false;
	okMsg("Player put on market");
}

void CLIMarketManager::onAddPlayerError(std::string const & reason)
{
	_waitForBid = false;
	errorMsg(reason);
}

void CLIMarketManager::onSalesUpdate()
{
	_waitForSales = false;
}

void CLIMarketManager::onMatchStart(){
	CLIMatchManager match(*this); 
	match.run();
}
