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

Client::Client(NetConfig const &config) : 	
												_user(),
												_installations(),
												_playersOnSale(),
												_players(),
												_connection(config.host, config.port),
												_userManager(_connection, _user),
												_matchManager(_connection, _user),
												_prompt(">"),
												_isRunning(true)
{
}

Client::~Client()
{
}

void Client::run()
{
	cout << Message::splashScreen();
	while (_isRunning == true)
	{
		if (_user.isLogged() == true)
		{
			mainMenu();
		}
		else
		{
			_isRunning = _userManager.displayMenu();
		}
	}
	cout << Message::goodBye();
}	

/* main menu */
void Client::mainMenu()
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
				managementMenu();
				break;
			case 2:
				marketMenu();
				break;
			case 3:
				friendlyMatchMenu();
				break;
			case 4:
				notificationsMenu();
			case 5:
				_user.logout();
			default:
				break;
		}
	}
	while (option != 5);
}

/* Management menu */
void Client::managementMenu()
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
				stadiumMenu();
				break;
			case 2:
				playersMenu();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

void Client::stadiumMenu()
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

void Client::playersMenu()
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
				printPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 1);
}

/* main menu */
void Client::notificationsMenu()
{
	_connection.updateNotifications();
	cout << "You have " << _connection.notifications.size() << "notifications." << endl;
	Menu _menu;
	_menu.addToDisplay("   - handle this notification\n");
	_menu.addToDisplay("   - see next notification\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool quit = false;
	while (_connection.notifications.available() && ! quit)
	{
		JSON::Value * currentNotification = _connection.notifications.front();
		JSON::Dict const & notif = DICT(currentNotification);
		if (STR(notif.get("type")).value() == net::MSG::FRIENDLY_GAME_INVITATION
			&& ISSTR(notif.get("data")))
		{
			cout << STR(notif.get("data")).value() << " invited you to a game" << endl;
		}
		option = _menu.run();
		switch(option)
		{
			case 1:
				handleNotification(currentNotification);
				_connection.notifications.pop();
				break;
			case 2:
				_connection.notifications.push(_connection.notifications.front());
				_connection.notifications.pop();
				break;
			case 3:
				quit = true;
				break;
			default:
				break;
		}
	}
}

//modif
void Client::printPlayers(){
	_players = getPlayers(_user.getUsername());//modif
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_players.size();++i){
		cout << _players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

/* Market menu */
void Client::marketMenu(){
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

void Client::salePlayer(){
	printPlayers();			//this function updates _players
	int player_id, bidValue;
	bool found = false;
	Player * player;
	cout << "Choose a player to sale by entering his ID :" <<endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_players.size(); ++i){
		if(_players[i].getMemberID() == player_id)
			player = &(_players[i]);
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
		addPlayerOnMarket(player_id, _user.getUsername(), bidValue);
		cout << "Your player was successfully added on market." << endl;
		}
		catch(playerAlreadyOnMarketException e){
			cout << "Error : you are already saling this player." << endl;
		}
	}
	else{
		cout << "Wrong ID." << endl;
	}
}

vector<int> Client::getBidValueRange(Player *player){
	int allowedRangeFromEstimatedValue = 10000; //TODO : in Constants.hpp (should do that for many others variables !)
	vector<int> range;
	range.push_back(player->estimatedValue() - (int) allowedRangeFromEstimatedValue/2);
	range.push_back(player->estimatedValue() + (int) allowedRangeFromEstimatedValue/2);
	return range;
}

void Client::printPlayersOnSale(){
	_playersOnSale = updatePlayersOnSale();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<_playersOnSale.size();++i){
		std::cout<<_playersOnSale[i]<<std::endl;
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

void Client::placeBid(){
	int player_id, value;
	string response;
	bool found=false;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_playersOnSale.size();++i){		//Getting the next bid value (which is in the JSON::Dict sent by server)
		if(player_id == _playersOnSale[i].getID()){
			found = true;
			value = _playersOnSale[i].getNextBidValue();
		}
	}
	if (found){
		try{
			bidOnPlayer(player_id, _user.getUsername(), value);
			
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
	}
	else{
		cout << "Error : this player is not in the list." << endl;
	}
}

//Market notifs


/* Friendly match menu */
void Client::friendlyMatchMenu()
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
				chooseUser();
				break;
			default:
				break;
		}
	}
	while(option != 3);
}

void Client::chooseUser()
{
	cout << "Enter a username to send an invitation to another user : ";
	string userInput;
	cin >> userInput;
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_USERNAME);
	toSend.set("data", userInput);
	_connection.send(toSend);

	cout << "Please wait for " << userInput << " to answer to your invitation..." << endl;
	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict const & received = DICT(serverMessage);
	if (ISDICT(received.get("data")) && ISSTR(DICT(received.get("data")).get("answer"))){
		string answer = STR(DICT(received.get("data")).get("answer")).value();
		if (answer == net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT){
			cout << STR(DICT(received.get("data")).get("username")).value();
			cout << " accepted your invitation!" << endl;
			startMatch();

		}
		else if (answer == net::MSG::USER_NOT_FOUND) {
			cout << "User not found" << endl;
		}
		else {
			cout << STR(DICT(received.get("data")).get("username")).value();
			cout << " denied your invitation. Sorry!" << endl;
		}
	}
}
void Client::loadInstallations(){
	_installations = getInstallationsList();
}

void Client::printInstallationsList(){
	if (_installations.empty())
	{
		loadInstallations();
	}
	// TODO implement printInstallationsList
	cout << "Here are all the installations you own :" << endl;
	for (size_t i = 0; i < _installations.size(); ++i){
		cout << i << " - " << _installations[i].getName() << endl;
		cout << "      Level : 				" << _installations[i].getLevel() << endl;
		cout << "      Current Value : 		" << _installations[i].getCurrentValue() << endl;
		cout << "      Upgrade Cost : 		" << _installations[i].getUpgradeCost() << endl;
		cout << "      Refund Ratio :       " << _installations[i].getRefundRatio() << endl;
		cout << "      Downgrade Refunds : 	" << _installations[i].getDowngradeRefunds() << endl;
	}
}

void Client::upgradeInstallation()
{
	size_t choice;
	cout << "Enter the number of the installation you want to upgrade" << endl << ">";
	cin >> choice;
	if (choice < _installations.size())
	{
		if (upgradeInstallation(choice))
		{
			_installations[choice].upgrade();
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
	if (choice < _installations.size())
	{
		if (downgradeInstallation(choice))
		{
			_installations[choice].downgrade();
		}
	}
	else
	{
		cout << "The number you entered is wrong" << endl;
	}
}

void Client::printConnectedUsersList(){
	vector<std::string> connectedUsers = getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
}


/* Private methods */

string Client::askForUserData(string prompt){
	string data;
	cout<<endl;
	cout << prompt;
	cin >> data;
	return data;
}

string Client::askForNewPassword(){
	string password = "a";
	string passwordConfirmation;
	while (password != passwordConfirmation){
		password = askForUserData("Enter a new password : ");
		passwordConfirmation = askForUserData("Confirm password : ");
		if (password != passwordConfirmation)
			cout << "The two passwords entered were not the same." << endl;
	}
	return password;
}

vector<Installation> Client::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATIONS_LIST);
	JSON::Dict const & response = DICT(serverResponse);
	
	vector<Installation> vec;
	if (ISLIST(response.get("data")))
	{
		toFill = LIST(response.get("data"));
		for (size_t i = 0; i < toFill.len(); ++i)
		{
			vec.push_back(DICT(toFill[i]));
		}
	}
	delete serverResponse;
	return vec;
}

bool Client::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATION_UPGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
	}
	delete serverResponse;
	return ret;
}

bool Client::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::INSTALLATION_DOWNGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
	}
	
	delete serverResponse;
	return ret;
}

vector<std::string> Client::getConnectedUsersList(){// TODO
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::CONNECTED_USERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISLIST(received.get("data"))) {
		JSON::List & connectedUsers = LIST(received.get("data"));
		for (size_t i = 0; i<connectedUsers.len(); ++i)
			res.push_back(STR(connectedUsers[i]));
	}
	return res;
}

///////// END NEW

std::vector<Sale> Client::updatePlayersOnSale(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	std::vector<Sale> res;
	
	if (ISLIST(received.get("data"))){
		JSON::List & sales = LIST(received.get("data"));
		for(size_t i = 0; i<sales.len();++i)
			res.push_back(Sale(DICT(sales[i]), Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)))));
	}
	return res; 
}

void Client::bidOnPlayer(int player_id,std::string username, int value){//modif
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
	}
}

void Client::addPlayerOnMarket(int player_id,std::string username, int value){
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
		}
}

std::vector<Player> Client::getPlayers(std::string username){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, username);//modif
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	_connection.send(query);
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::PLAYERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	JSON::List toFill;
	if(ISLIST(received.get("data"))){
		toFill = LIST(received.get("data"));
	}
	vector<Player> myplayers;
	for(size_t i=0; i<toFill.len();++i){
		Player player(DICT(toFill[i]));
		myplayers.push_back(player);
	}
	delete serverResponse;
	return myplayers;
}


void Client::handleNotification(JSON::Value *notification){
	JSON::Dict message = DICT(notification);
	string messageType;
	if (ISSTR(message.get("type")))
		messageType = STR(message.get("type")).value();

		if (messageType == net::MSG::FRIENDLY_GAME_INVITATION)
			handleFriendlyGameInvitation(message);
		if (messageType == net::MSG::MARKET_MESSAGE)
			handleEndOfSaleNotification(DICT(message.get("data")));
}

void Client::handleFriendlyGameInvitation(JSON::Dict &message){
	if (ISSTR(message.get("data"))){
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
					acceptInvitationFromUser(STR(message.get("data")).value());
					// TODO start to play game
					break;
				case 2:
					chosen = true;
					denyInvitationFromUser(STR(message.get("data")).value());
					break;
				default:
					break;
			}
		}
		while(!chosen);
	}
}

void Client::acceptInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT);
	toSend.set("data", data);
	_connection.send(toSend);
	startMatch();
}

void Client::denyInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_DENY);
	toSend.set("data", data);
	_connection.send(toSend);
}

void Client::startMatch(){
	JSON::Value *serverBalls = _connection.waitForMsg(net::MSG::MATCH_BALLS);
	JSON::Dict const &receivedBalls = DICT(serverBalls);
	_matchManager.initBalls(receivedBalls);
	JSON::Value *serverSquads = _connection.waitForMsg(net::MSG::MATCH_SQUADS);
	JSON::Dict const &receivedSquads = DICT(serverSquads);
	_matchManager.initSquads(receivedSquads);
	//_connection.waitForMsg(net::MSG::MATCH_START);
	_matchManager.startMatch();
}


/*Market notifications*/

void Client::handleEndOfSaleNotification(JSON::Dict & json){
	cout << "<<MESSAGE : SALE ENDED>>" << endl;
	if(STR(json.get("type")).value()==net::MSG::END_OF_OWNED_SALE_RAPPORT){
		if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_NOT_SOLD){
			cout << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << "was not sold. :(" << endl; 
		}
		else if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_SOLD){
			cout << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has been sold for " << INT(json.get(net::MSG::BID_VALUE)) 
			<< " to " << STR(json.get(net::MSG::CURRENT_BIDDER)).value() << endl;
		}
	}
	else if(STR(json.get("type")).value()==net::MSG::WON_SALE_RAPPORT){
		cout << "You bought player " << INT(json.get(net::MSG::PLAYER_ID)) << " for " << INT(json.get(net::MSG::BID_VALUE)) << "." <<endl;
		cout << "This player comes from " << STR(json.get(net::MSG::SALE_OWNER)).value() << "'s team." << endl;
	}
}
