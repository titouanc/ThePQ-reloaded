#include "Client.hpp"
#include <pthread.h>
#include "CLIMatchManager.hpp"

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Client::Client(NetConfig const &config) : 	_user(), _connection(config.host, config.port),
												_userManager(_connection, _user),
												_matchManager(_connection, _user),
												_teamManager(_connection, _user),
												_marketManager(_connection, _user),
												_notificationManager(_connection, _user),
												_isRunning(true),
												_prompt(">")
{
	_connection.run();
	_notificationManager.addCallback(pair<string, AbstractCallback*>(net::MSG::FRIENDLY_GAME_INVITATION, new ClassCallback<Client>(this, &Client::handleFriendlyGameInvitation)));
	_notificationManager.addCallback(pair<string, AbstractCallback*>(net::MSG::MARKET_MESSAGE, new ClassCallback<Client>(this, &Client::handleEndOfSaleNotification)));
	_notificationManager.addCallback(pair<string, AbstractCallback*>(net::MSG::TEAM_INFOS, new ClassCallback<NotificationManager>(&_notificationManager, &NotificationManager::loadTeam)));
}

Client::~Client()
{
}

void Client::run()
{
	cout << splashScreen();
	_userManager.run();
	cout << goodBye();
}	

/* main menu */


/* main menu */
void Client::showNotificationsMenu()
{
	_connection.updateNotifications();
	cout << "You have " << _connection.getNotifications().size() << " notifications." << endl;
	_notificationManager.handleAllNotifications();
}

/* Friendly match menu */
void Client::showFriendlyMatchMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - list all connected players\n");
	_menu.addToDisplay("   - choose one to play a friendly game with\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				printConnectedUsersList();
				break;
			case 2:
				showChooseUserMenu();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

void Client::showChooseUserMenu()
{
	string userInput = Menu::askForUserData("Enter a username to send an invitation to another user : ");
	_matchManager.chooseUser(userInput);
	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
	try {
		_matchManager.waitForUser();
		cout << userInput << " accepted your invitation!" << endl;
		_matchManager.startMatch();
		showTurnMenu();
	}
	catch (UserNotFoundException &e)
	{
		cout << "Username not found" << endl;
	}
	catch (UserDeniedException &e)
	{
		cout << userInput << " denied your invitation. Sorry!" << endl;		
	}
}

void Client::printConnectedUsersList(){
	vector<std::string> connectedUsers = _matchManager.getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
}

bool Client::askForNotificationHandling()
{
	Menu _menu;
	_menu.addToDisplay("   - handle this notification\n");
	_menu.addToDisplay("   - see next notification\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				return true;
				break;
			case 2:
				return false;
				break;
			default:
				cout << "Wrong option entered" << endl;
				break;
		}
	}
	while (true);
}

void Client::handleFriendlyGameInvitation(JSON::Value const *json){
	if (ISSTR(json)){
		string user = STR(json).value();
		cout << user << " invited you to a game" << endl;
		bool handle = askForNotificationHandling();
		Menu _menu;
		_menu.addToDisplay("   - accept\n");
		_menu.addToDisplay("   - deny\n");
		int option;
		while(handle)
		{
			option = _menu.run();
			if (option == 1){
				handle = false;
				
				//_matchManager.acceptInvitationFromUser(user);
				//showTurnMenu();
				// TODO start to play game
				CLIMatchManager YAYAYA(_connection, _user);
				YAYAYA.run();
				break;
			} else if (option == 2){
				handle = false;
				_matchManager.denyInvitationFromUser(user);
				break;
			} else {
				cout << "Wrong option entered" << endl;
				break;
			}
		}
	}
}

/*Market notifications*/

void Client::handleEndOfSaleNotification(JSON::Value const * message){
	JSON::Dict const & json = DICT(message);
	cout << "\n\033[36mMessage : a sale has ended.\033[0m" << endl;
	if(STR(json.get("type")).value()==net::MSG::END_OF_OWNED_SALE_RAPPORT){
		if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_NOT_SOLD){
			cout << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has not been sold." << endl; 
		}
		else if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_SOLD){
			cout << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has been sold for " << INT(json.get(net::MSG::BID_VALUE)) 
			<< " to " << STR(json.get(net::MSG::CURRENT_BIDDER)).value() << endl;
		}
	}
	else if(STR(json.get("type")).value()==net::MSG::WON_SALE_RAPPORT){
		std::string owner = STR(json.get(net::MSG::SALE_OWNER)).value();
		cout << "You bought player " << INT(json.get(net::MSG::PLAYER_ID)) << " for \33[32m" << INT(json.get(net::MSG::BID_VALUE)) << "\033[0m." <<endl;
		if(owner==net::MSG::GENERATED_BY_MARKET)
			cout << "This player did not belong to any team. He was free. Like the wind."<<endl;
		else
			cout << "This player comes from " << owner << "'s team." << endl;
	}
	cout<<endl;
}


string Client::splashScreen(){
	string message;
	message+="	             _____ _            ____            \n";
	message+="	            |_   _| |__   ___  |  _ \\ _ __ ___ \n";
	message+="		      | | | '_ \\ / _ \\ | |_) | '__/ _ \\\n ";
	message+="		      | | | | | |  __/ |  __/| | | (_) |\n";
	message+="		      |_| |_| |_|\\___| |_|   |_|  \\___/ \n";
	message+="		   ___        _     _     _ _ _       _ \n";
	message+="		  / _ \\ _   _(_) __| | __| (_) |_ ___| |__ \n";
	message+="		 | | | | | | | |/ _` |/ _` | | __/ __| '_ \\ \n";
	message+="		 | |_| | |_| | | (_| | (_| | | |_ (__| | | |\n";
	message+="		  \\__\\_ \\__,_|_|\\__,_|\\__,_|_|\\__\\___|_| |_|\n";
	message+= "        =======||\\    	Just a game, no bullshit!\n";
	message+= "    ===========|| \\___________________________  ___            |\n";
	message+= "  =============|| |___________________________==___|>        - * -\n";
	message+= "    ===========|| /                                            |\n";
	message+= "        =======||/ 		\n";
	message+="		  _ __ ___   __ _ _ __   __ _  __ _  ___ _ __ \n";
	message+="		 | '_ ` _ \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|\n";
	message+="		 | | | | | | (_| | | | | (_| | (_| |  __/ |\n";
	message+="		 |_| |_| |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|\n";
	message+="   		               	              |___/\n";
	message+="			Welcome to The Pro Quidditch Manager 2014!\n";
return message;
}

string Client::goodBye(){
	string message;
	message+= "                 =========================            \n";
	message+= "   Thank you for playing the Pro Quidditch Manager 2014!\n";
	message+= "                   See you next time! :)\n";
	message+= "                 =========================            \n";
	return message;
}

// Users


// Team
void Client::showTeamMenu()
{
	Menu _menu;
	_menu.addToDisplay("	- Show list of players");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				displayPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 2);
}

void Client::displayPlayers(){
	_teamManager.loadPlayers();
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_user.players.size();++i){
		cout << _user.players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

// Market
void Client::showMarketMenu(){
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

void Client::sellPlayer(){
	displayPlayers();			//this function updates _players
	int player_id, bidValue;
	bool found = false;
	Player * player;
	cout << "Choose a player to sale by entering his ID :" <<endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_user.players.size(); ++i){
		if(_user.players[i].getMemberID() == player_id){
			player = &(_user.players[i]);
			found = true;
		}
	}
	if(found){
		pair<int, int> range = _marketManager.getBidValueRange(player);
		cout << "Enter the starting bid value (must be between " << range.first << " and " << range.second << ") :" << endl;
		cout << _prompt;
		cin >> bidValue;
		while(bidValue<range.first or bidValue>range.second){
			cout << bidValue << " is not between " << range.first << " and " << range.second << " !\nTry again :" << endl;//modif
			cout << _prompt;
			cin >> bidValue;
		}
		try{
			_marketManager.addPlayerOnMarket(player_id, bidValue);
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
void Client::displayPlayersOnSale(){
	_marketManager.updateSales();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<_marketManager.getSales().size();++i){
		std::cout<<_marketManager.getSales()[i]<<std::endl;
	}
	cout << "=================================================" << endl;
}

void Client::showBidMenu(){
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

void Client::placeBid(){
	int player_id;
	string response;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	cout << _prompt;
	cin >> player_id;
	try{
		_marketManager.bidOnPlayer(player_id);
		
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

// Match

void Client::displayAvailablePlayers(){
	for (int i=0; i<7; ++i){
		cout << "\t- " << playerLetter(*(_matchManager.getOwnSquad().players[i])) << " ";
		cout << _matchManager.getOwnSquad().players[i]->getName() << endl;
	}
}

Position Client::parseDirection(string userInput){
	Position res(0,0);
	if (userInput.size() == 1){
		if (userInput == "e")
			res = Pitch::East;
		else if (userInput == "w")
			res = Pitch::West;
	}
	else if (userInput.size() == 2){
		if (userInput == "nw")
			res = Pitch::NorthWest;
		else if (userInput == "ne")
			res = Pitch::NorthEast;
		else if (userInput == "sw")
			res = Pitch::SouthWest;
		else if (userInput == "se")
			res = Pitch::SouthEast;
	}
	return res;
}

char Client::playerLetter(Player const & player)
{
	return 'A' + player.getID() - 1;
}

std::string Client::colorPlayerLetter(Player const & player)
{
	char res[7] = "\033[30mX";
	if (player.isSeeker())
		res[3] = '3';
	else if (player.isChaser())
		res[3] = '4';
	else if (player.isBeater())
		res[3] = '1';
	else if (player.isKeeper())
		res[3] = '6';
	res[5] = playerLetter(player);
	return std::string(res);
}

void Client::displayPitch()
{
	Moveable *atPos = NULL;
	Pitch _pitch = _matchManager.getPitch();
	for (int y=_pitch.ymax()-1; y>=_pitch.ymin(); y--){
		for (int x=_pitch.xmin(); x<_pitch.xmax(); x++){

			/* background */
			if (_pitch.isInKeeperZone(x, y))
				cout << "\033[47m";
			else if (_pitch.inEllipsis(x, y))
				cout << "\033[42m";

			/* foreground */
			if (! _pitch.isValid(x, y) || ! _pitch.inEllipsis(x, y)){
				cout << ' ';
			} else {
				atPos = _pitch.getAt(x, y);
				if (_pitch.isGoal(x, y)){
					cout << "\033[1m\033[44mO";
				} else if (atPos==NULL){
					cout << '.';
				} else if (atPos->isBall()){
					/* Colorize balls by type */
					Ball const & ball = (Ball const &) *atPos;
					cout << "\033[1m";
					if (ball.isGoldenSnitch())
						cout << "\033[33m";
					else if (ball.isQuaffle())
						cout << "\033[34m";
					else if (ball.isBludger())
						cout << "\033[31m";
					cout << '*';
				} else if (atPos->isPlayer()){
					/* Colorize players by type */
					Player const & player = (Player const &) *atPos;
					
					if (_matchManager.isOwnPlayer(player))
						cout << "\033[1m";
					cout << colorPlayerLetter(player);
				}
			}
			cout << "\033[0m";
		}
		cout << endl;
	}
	cout << "\033[1m"
		 << "\033[34mChaser \033[31mBeater \033[36mKeeper \033[33mSeeker"
	     << "\033[34m*Quaffle \033[31m*Bludger \033[33m*Golden Snitch\033[0m"
		 << endl;
}


void Client::showTurnMenu(){
	
	Menu showTurnMenu;
	showTurnMenu.addToDisplay("\t- select player");
	int option;
	do {
		_matchManager.updatePitchWithDeltas();
		displayPitch();
		displayAvailablePlayers();
		option = showTurnMenu.run();
		switch(option){
			case 1:
				selectPlayer();
				break;
			default:
				break;
		}
		_connection.updateNotifications();
		_matchManager.updatePitchWithDeltas();
		displayPitch();
	} while (!_matchManager.isMatchFinished());
}

void Client::selectPlayer(){
	Menu selectPlayer;
	for (int i=0; i<7; ++i){
		selectPlayer.addToDisplay(
			std::string("\t- \033[1m") + 
			colorPlayerLetter(*(_matchManager.getOwnSquad().players[i])) +
			" - " +
			_matchManager.getOwnSquad().players[i]->getName() +
			"\033[0m"
		);
	}
	selectPlayer.addToDisplay("\t- back");
	int option = 0;
	if ((option = selectPlayer.run()) != 8){
		selectDirectionForPlayer(option);
	}
}

void Client::selectDirectionForPlayer(int player){
	Displacement currentDisplacement;
	Menu selectDirection;
	selectDirection.addToDisplay("\t- NorthEast");
	selectDirection.addToDisplay("\t- East");
	selectDirection.addToDisplay("\t- SouthEast");
	selectDirection.addToDisplay("\t- SouthWest");
	selectDirection.addToDisplay("\t- West");
	selectDirection.addToDisplay("\t- NorthWest");
	selectDirection.addToDisplay("\t- done");
	int option = 0;
	while ((option = selectDirection.run()) != 7){
		switch (option){
			case 1:
				currentDisplacement.addMove(Pitch::NorthEast);
				break;
			case 2:
				currentDisplacement.addMove(Pitch::East);
				break;
			case 3:
				currentDisplacement.addMove(Pitch::SouthEast);
				break;
			case 4:
				currentDisplacement.addMove(Pitch::SouthWest);
				break;
			case 5:
				currentDisplacement.addMove(Pitch::West);
				break;
			case 6:
				currentDisplacement.addMove(Pitch::NorthWest);
				break;
			default:
				break;
		}
	}
	_matchManager.sendStroke(player, currentDisplacement);
}
