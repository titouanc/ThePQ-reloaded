#include "Client.hpp"
#include <pthread.h>

using namespace std;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Client::Client(NetConfig const &config) : 	_user(),
												_connection(config.host, config.port),
												_userManager(_connection, _user),
												_stadiumManager(_connection, _user),
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
	while (_isRunning == true)
	{
		if (_user.isLogged() == true)
		{
			showMainMenu();
		}
		else
		{
			_isRunning = showUserMenu();
		}
	}
	cout << goodBye();
}	

/* main menu */
void Client::showMainMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - manage your team and stadium\n");
	_menu.addToDisplay("   - access market\n");
	_menu.addToDisplay("   - play a friendly game\n");
	_menu.addToDisplay("   - see notifications\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				showManagementMenu();
				break;
			case 2:
				showMarketMenu();
				break;
			case 3:
				showFriendlyMatchMenu();
				break;
			case 4:
				showNotificationsMenu();
				break;
			case 5:
				_userManager.logoutUser();
			default:
				break;
		}
	}
	while (option != 5);
}

/* Management menu */
void Client::showManagementMenu()
{
	Menu _menu;
	_menu.addToDisplay("   - manage your stadium and installations\n");
	_menu.addToDisplay("   - manage your players\n");
	_menu.addToDisplay("   - quit to main menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				showStadiumMenu();
				break;
			case 2:
				showTeamMenu();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}


/* main menu */
void Client::showNotificationsMenu()
{
	_connection.updateNotifications();
	cout << "You have " << _connection.getNotifications().size() << " notifications." << endl;
	Menu _menu;
	_menu.addToDisplay("   - handle this notification\n");
	_menu.addToDisplay("   - see next notification\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool quit = false;
	while (_connection.getNotifications().available() && ! quit)
	{
		JSON::Value * currentNotification = _connection.getNotifications().front();
		JSON::Dict const & notif = DICT(currentNotification);
		string messageType = STR(notif.get("type")).value();
		if (messageType == net::MSG::FRIENDLY_GAME_INVITATION && ISSTR(notif.get("data")))
		{
			cout << STR(notif.get("data")).value() << " invited you to a game" << endl;
		}
		else if (messageType == net::MSG::MARKET_MESSAGE)
		{
			cout << "You won a bid !" << endl;
		}
		option = _menu.run();
		switch(option)
		{
			case 1:
				_notificationManager.handleNotification(currentNotification);
				_connection.getNotifications().pop();
				break;
			case 2:
				_connection.getNotifications().push(_connection.getNotifications().front());
				_connection.getNotifications().pop();
				break;
			case 3:
				quit = true;
				break;
			default:
				break;
		}
	}
}

/* Market menu */


//Market notifs


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



///////// END NEW



void Client::handleFriendlyGameInvitation(JSON::Value const *json){
	if (ISSTR(json)){
		string user = STR(json).value();
		Menu _menu;
		_menu.addToDisplay("   - accept\n");
		_menu.addToDisplay("   - deny\n");
		int option;
		bool chosen = false; // user has to accept or deny, he cannot quit.
		do
		{
			option = _menu.run();
			switch(option)
			{
				case 1:
					chosen = true;
					_matchManager.acceptInvitationFromUser(user);
					showTurnMenu();
					// TODO start to play game
					break;
				case 2:
					chosen = true;
					_matchManager.denyInvitationFromUser(user);
					break;
				default:
					break;
			}
		}
		while(!chosen);
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

bool Client::showUserMenu()
{
	/* user menu */
	Menu _menu;
	_menu.addToDisplay("   - login\n");
	_menu.addToDisplay("   - register\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool res = true;
	option = _menu.run();
	switch(option)
	{
		case 1:
			showLoginMenu();
			break;
		case 2:
			showRegisterMenu();
			break;
		default:
			res = false;
			break;
	}
	return res;
}

void Client::showLoginMenu()
{	
	string username = Menu::askForUserData("Username : ");
	string password = Menu::askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		_userManager.loginUser(username, password);
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		//~ showMainMenu();
	}
	catch (NoTeamNameException e)
	{
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		showTeamNameMenu();
	}
	catch (UserNotFoundException & e)
	{
		cout << "\nUser not found" << endl;
	}
	catch (WrongPasswordException & e)
	{
		cout << "\nWrong password" << endl;
	}
	catch (AlreadyLoggedInException & e)
	{
		cout << "\nYou're already logged in from another location" << endl;
	}
}

void Client::showTeamNameMenu(){
	bool found = false;
	cout << "Hey, it's the first time you log in ! Please, pick up a name for your team." << endl;
	do{
		string teamname = Menu::askForUserData("Teamname : ");
		try{
			_userManager.chooseTeamName(_user.username,teamname);
			cout << "You have successfully picked up a name for your team !\nYou are now the manager of \033[35m"<<teamname<<"\033[0m"<< " !"<<endl;
			found = true;
		}
		catch(TeamNameNotAvailableException e){
			cout << "That teamname is \033[31mnot\033[0m available, pick up another one." << endl;
		}
	}while(!found);
}

void Client::showRegisterMenu()
{
	bool registered = false;
	for (int i = 0; i < 3 && ! registered; ++i)
	{
		string username = Menu::askForUserData("Pick a username : ");
		try {
			cout << "Please wait..." << endl;
			_userManager.doesUserExist(username);
			string password = "a";
			string passwordConfirmation;
			while (password != passwordConfirmation){
				password = Menu::askForUserData("Enter a new password : ");
				passwordConfirmation = Menu::askForUserData("Confirm password : ");
				if (password != passwordConfirmation)
					cout << "The two passwords entered were not the same." << endl;
			}
			cout << "Please wait..." << endl;
			_userManager.registerUser(username, password);
			registered = true;
			cout << "You have successfully registered! You can now login." << endl;
		}
		catch (UserAlreadyExistsException e) {
			cout << "Username already exists. Try again with a different username." << endl;		
		}
	}
}

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


void Client::showStadiumMenu()
{
	Menu _menu;
	_menu.addToDisplay("    - view your installations\n");
	_menu.addToDisplay("    - upgrade an installation\n");
	_menu.addToDisplay("    - downgrade an installation\n");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				printInstallationsList();
				break;
			case 2:
				upgradeInstallation();
				break;
			case 3:
				downgradeInstallation();
				break;
			default:
				break;
		}
	}
	while (option != 4);
}

void Client::printInstallationsList(){
	if (_user.installations.empty())
	{
		_stadiumManager.loadInstallations();
	}
	// TODO implement printInstallationsList
	cout << "You have " << _user.funds << "$$$$" << endl;
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < _user.installations.size(); ++i){
		cout << i << " - " << _user.installations[i]->getName() << endl;
		cout << "      Level : 				" << _user.installations[i]->getLevel() << endl;
		cout << "      Current Value : 		" << _user.installations[i]->getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << _user.installations[i]->getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << _user.installations[i]->getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << _user.installations[i]->getDowngradeRefunds() << endl;
	}
}

void Client::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl << ">";
	cin >> choice;
	if (choice < _user.installations.size())
	{
		if (_stadiumManager.upgradeInstallation(choice))
		{
			_user.funds -= _user.installations[choice]->getUpgradeCost();
			_user.installations[choice]->upgrade();
		}
		else
		{
			cout << "You have insufficient funds" << endl;
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void Client::downgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to downgrade" << endl << ">";
	cin >> choice;
	if (choice < _user.installations.size())
	{
		if (_stadiumManager.downgradeInstallation(choice))
		{
			_user.funds += _user.installations[choice]->getDowngradeRefunds();
			_user.installations[choice]->downgrade();
		}
		else
		{
			cout << "You can not downgrade this installation because it is not build yet !" << endl;
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}
