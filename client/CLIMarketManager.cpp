#include "CLIMarketManager.hpp"

void CLIMarketManager::run(){
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

void CLIMarketManager::sellPlayer(){
	// displayPlayers();			//this function updates _players
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
		cout << _prompt;
		cin >> bidValue;
		while(bidValue<range.first or bidValue>range.second){
			cout << bidValue << " is not between " << range.first << " and " << range.second << " !\nTry again :" << endl;//modif
			cout << _prompt;
			cin >> bidValue;
		}
		try{
			addPlayerOnMarket(player_id, bidValue);
			cout << "Your player was successfully added on market." << endl;
		}
		catch(playerAlreadyOnMarketException e){
			cout << "\033[1;31mError\033[0m : you are already selling this player." << endl;
		}
		catch(notEnoughPlayersException e){
			cout<< "\033[1;31mError\033[0m : you do not have enough players to put a player on sale. You need at least " << gameconfig::MIN_PLAYERS + 1<<" players to do so."<<std::endl;
		}
	}
	else{
		cout << "Wrong ID." << endl;
	}
}
void CLIMarketManager::displayPlayersOnSale(){
	updateSales();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<getSales().size();++i){
		std::cout<<getSales()[i]<<std::endl;
	}
	cout << "=================================================" << endl;
}

void CLIMarketManager::showBidMenu(){
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

void CLIMarketManager::placeBid(){
	int player_id;
	string response;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	prompt();
	cin >> player_id;
	try{
		bidOnPlayer(player_id);
		
		cout << "Bid successfully placed ! Hurra !" << endl;
	}
	catch(PlayerNotFoundException& e) {
		cout << "\033[1;31mError\033[0m : the player id you entered is not correct" << endl;
	}
	catch(bidValueNotUpdatedException e){
		cout << "\033[1;31mError\033[0m : bid value not correct (update your market list)."<<endl;
	}
	catch(turnException e){
		cout << "\033[1;31mError\033[0m : you did not bid last turn."<<endl;
	}
	catch(bidEndedException e){
		cout << "\033[1;31mError\033[0m : player not on market any more (update your market list)."<<endl;
	}
	catch(bidOnYourPlayerException e){
		cout << "\033[1;31mError\033[0m : cannot bid on your player."<<endl;
	}
	catch(lastBidderException e){
		cout << "\033[1;31mError\033[0m : you are currently winning this sale. Cannot bid on your own bid."<<endl;
	}
	catch(tooManyPlayersException e){
		cout << "\033[1;31mError\033[0m : you have too many players to be able to place a bid. You cannot have more than "<<gameconfig::MAX_PLAYERS<<" players."<<endl;
	}
	catch(insufficientFundsException e){
		cout << "\033[1;31mError\033[0m : not enough money (GET MORE $$$$$)."<<endl;
	}
}