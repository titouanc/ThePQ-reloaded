#include "MarketManager.hpp"

MarketManager::MarketManager(net::ClientConnectionManager& connection, UserData& user, TeamManager& teamManager) :
	_connection(connection), _user(user), _teamManager(teamManager), _prompt('>'), _sales()
{}

void MarketManager::showMenu(){
	Menu _menu;
	_menu.addToDisplay("   - put a player on sale\n");
	_menu.addToDisplay("   - see the players on sale\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				salePlayer();
				break;
			case 2:
				printPlayersOnSale();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

void MarketManager::salePlayer(){
	_teamManager.printPlayers();			//this function updates _players
	int player_id, bidValue;
	bool found = false;
	Player * player;
	cout << "Choose a player to sale by entering his ID :" <<endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_user.players.size(); ++i){
		if(_user.players[i].getMemberID() == player_id)
			player = &(_user.players[i]);
			found = true;
	}
	if(found){
		vector<int> range = getBidValueRange(player);
		cout << "Enter the starting bid value (must be between " << range[0] << " and " << range[1] << ") :" << endl;
		cout << _prompt;
		cin >> bidValue;
		while(bidValue<range[0] or bidValue>range[1]){
			cout << bidValue << " is not between " << range[0] << " and " << range[1] << " !\nTry again :" << endl;//modif
			cout << _prompt;
			cin >> bidValue;
		}
		try{
			addPlayerOnMarket(player_id, _user.username, bidValue);
			cout << "Your player was successfully added on market." << endl;
		}
		catch(playerAlreadyOnMarketException e){
			cout << "Error : you are already saling this player." << endl;
		}
		catch(notEnoughPlayersException e){
			cout<<"Error : you do not have enough players to put a player on sale. You need at least " << gameconfig::MIN_PLAYERS + 1<<" players to do so."<<std::endl;
		}
	}
	else{
		cout << "Wrong ID." << endl;
	}
}

vector<int> MarketManager::getBidValueRange(Player *player){
	int allowedRangeFromEstimatedValue = 10000; //TODO : in Constants.hpp (should do that for many others variables !)
	vector<int> range;
	range.push_back(player->estimatedValue() - (int) allowedRangeFromEstimatedValue/2);
	range.push_back(player->estimatedValue() + (int) allowedRangeFromEstimatedValue/2);
	return range;
}

void MarketManager::printPlayersOnSale(){
	updateSales();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<_sales.size();++i){
		std::cout<<_sales[i]<<std::endl;
	}
	cout << "=================================================" << endl;
	Menu _menu;
	_menu.addToDisplay("   - place a bid on a player\n");
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
			default:
				break;
		}
	}
	while (option != 2);
}

void MarketManager::placeBid(){
	int player_id, value;
	string response;
	bool found=false;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_sales.size();++i){		//Getting the next bid value (which is in the JSON::Dict sent by server)
		if(player_id == _sales[i].getID()){
			found = true;
			value = _sales[i].getNextBidValue();
		}
	}
	if (found){
		try{
			bidOnPlayer(player_id, _user.username, value);
			
			cout << "Bid successfully placed ! Hurra !" << endl;
			cout << "Updated list :" << endl;
			printPlayersOnSale();
		}
		catch(bidValueNotUpdatedException e){
			cout << "Error : bid value not correct (update your market list)."<<endl;
		}
		catch(turnException e){
			cout << "Error : you did not bid last turn."<<endl;
		}
		catch(bidEndedException e){
			cout << "Error : player not on market any more (update your market list)."<<endl;
		}
		catch(bidOnYourPlayerException e){
			cout << "Error : cannot bid on your player."<<endl;
		}
		catch(lastBidderException e){
			cout << "Error : you are currently winning this sale. Cannot bid on your own bid."<<endl;
		}
		catch(tooManyPlayersException e){
			cout << "Error : you have too many players to be able to place a bid. You cannot have more than "<<gameconfig::MAX_PLAYERS<<" players."<<endl;
		}
		catch(insufficientFundsException e){
			cout << "Error : not enough money (GET MORE $$$$$)."<<endl;
		}
	}
	else {
		cout << "Error : this player is not in the list." << endl;
	}
}

void MarketManager::updateSales(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	
	_sales.clear();
	if (ISLIST(received.get("data"))){
		JSON::List & sales = LIST(received.get("data"));
		for(size_t i = 0; i<sales.len();++i)
			_sales.push_back(Sale(DICT(sales[i]), Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)))));
	}
}

void MarketManager::bidOnPlayer(int player_id,std::string username, int value){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	query.set("data", data);
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::BID_ON_PLAYER_QUERY);
	JSON::Dict const & received = DICT(serverResponse);
	
	if(ISSTR(received.get("data"))){
		std::string res = STR(received.get("data")).value();
		if(res == net::MSG::BID_VALUE_NOT_UPDATED)
			throw bidValueNotUpdatedException();
		else if(res == net::MSG::BID_TURN_ERROR)
			throw turnException();
		else if(res == net::MSG::BID_ENDED)
			throw bidEndedException();
		else if(res == net::MSG::CANNOT_BID_ON_YOUR_PLAYER)
			throw bidOnYourPlayerException();
		else if(res == net::MSG::LAST_BIDDER)
			throw lastBidderException();
		else if(res == net::MSG::TOO_MANY_PLAYERS)
			throw tooManyPlayersException();
		else if(res == net::MSG::INSUFFICIENT_FUNDS)
			throw insufficientFundsException();
	}
}

void MarketManager::addPlayerOnMarket(int player_id,std::string username, int value){
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	JSON::Dict const & received = DICT(serverResponse);
		if(ISSTR(received.get("data"))){
			std::string res = STR((received.get("data"))).value();
			if(res == net::MSG::PLAYER_ALREADY_ON_MARKET)
				throw playerAlreadyOnMarketException();
			else if(res == net::MSG::NOT_ENOUGH_PLAYERS)
				throw notEnoughPlayersException();
		}
}
