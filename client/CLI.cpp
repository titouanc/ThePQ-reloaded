#include "CLI.hpp"
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

CLI::CLI(NetConfig const &config) : 	_connection(config.host, config.port),
										_prompt(">"),
										_isWaitingForMessage(false)
{
	pthread_create(&_thread, NULL, net::runThread, this);
}

CLI::~CLI()
{
	
}

void CLI::run()
{
	cout << Message::splashScreen();
	
	loginMenu();
	
	cout << Message::goodBye();
}

void CLI::loginMenu()
{
	/* user menu */
	Menu _menu;
	_menu.addToDisplay("   - login\n");
	_menu.addToDisplay("   - register\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				login();
				break;
			case 2:
				registerUser();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

void CLI::login(){
	
	string username = askForUserData("Username : ");
	string password = askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		loginUser(username, password);
		_username = username;//modif
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		mainMenu();
	}
	catch (UserNotFoundException e)
	{
		cout << "\nUser not found" << endl;
	}
	catch (WrongPasswordException e)
	{
		cout << "\nWrong password" << endl;
	}
}

void CLI::registerUser(){
	bool registered = false;
	for (int i = 0; i < 3 && ! registered; ++i)
	{
		string username = askForUserData("Pick a username : ");
		try {
			cout << "Please wait..." << endl;
			doesUserExist(username);
			string password = askForNewPassword();
			cout << "Please wait..." << endl;
			registerUser(username, password);
			registered = true;
			cout << "You have successfully registered! You can now login." << endl;
		}
		catch (UserAlreadyExistsException e) {
			cout << "User name already exists. Try again with a different username." << endl;		
		}
	}
}
	

/* main menu */
void CLI::mainMenu()
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
			default:
				break;
		}
	}
	while (option != 5);
}

/* Management menu */
void CLI::managementMenu()
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

void CLI::stadiumMenu()
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

void CLI::playersMenu()
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
void CLI::notificationsMenu()
{
	updateNotifications();
	displayNotificationsCount();
	Menu _menu;
	_menu.addToDisplay("   - handle this notification\n");
	_menu.addToDisplay("   - see next notification\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	bool quit = false;
	while (! _messages.empty() && ! quit)
	{
		JSON::Value * currentNotification = _messages.front();
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
				_messages.pop();
				break;
			case 2:
				_messages.push(_messages.front());
				_messages.pop();
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
void CLI::printPlayers(){
	_players = getPlayers(_username);//modif
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_players.size();++i){
		cout << _players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}

/* Market menu */
void CLI::marketMenu(){
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

void CLI::salePlayer(){
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
		addPlayerOnMarket(player_id, _username, bidValue);
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

vector<int> CLI::getBidValueRange(Player *player){
	int allowedRangeFromEstimatedValue = 10000; //TODO : in Constants.hpp (should do that for many others variables !)
	vector<int> range;
	range.push_back(player->estimatedValue() - (int) allowedRangeFromEstimatedValue/2);
	range.push_back(player->estimatedValue() + (int) allowedRangeFromEstimatedValue/2);
	return range;
}

void CLI::printPlayersOnSale(){
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

void CLI::placeBid(){
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
			bidOnPlayer(player_id, _username, value);
			
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

/* Friendly match menu */
void CLI::friendlyMatchMenu()
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

void CLI::chooseUser()
{
	cout << "Enter a username to send an invitation to another user : ";
	string userInput;
	cin >> userInput;
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_USERNAME);
	toSend.set("data", userInput);
	_connection.send(toSend);

	JSON::Value *serverMessage = waitForMsg(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict const & received = DICT(serverMessage);
	if (ISDICT(received.get("data")) && ISSTR(DICT(received.get("data")).get("answer"))){
		string answer = STR(DICT(received.get("data")).get("answer")).value();
		if (answer == net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT){
			cout << STR(DICT(received.get("data")).get("username")).value();
			cout << " accepted your invitation!" << endl;
			// launch clientmatchmanager

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
void CLI::loadInstallations(){
	_installations = getInstallationsList();
}

void CLI::printInstallationsList(){
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

void CLI::upgradeInstallation()
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

void CLI::downgradeInstallation()
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

void CLI::printConnectedUsersList(){
	vector<std::string> connectedUsers = getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
}


/* Private methods */

string CLI::askForUserData(string prompt){
	string data;
	cout<<endl;
	cout << prompt;
	cin >> data;
	return data;
}

string CLI::askForNewPassword(){
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

////////// NEW 
void CLI::loginUser(string username, string passwd){

	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = waitForMsg(net::MSG::STATUS);
	JSON::Dict const & received = DICT(serverMessage); 	// receiving server response
	if (ISSTR(received.get("data"))) {
		if (STR(received.get("data")).value() == net::MSG::PASSWORD_ERROR)
		{
			delete serverMessage;
			throw WrongPasswordException();
		}
		else if (STR(received.get("data")).value() == net::MSG::USER_NOT_FOUND)
		{
			delete serverMessage;
			throw UserNotFoundException();
		}
	}
	delete serverMessage;
	// User is logged in at this point.
}

void CLI::doesUserExist(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS);
	toSend.set("data", username);
	_connection.send(toSend);

	JSON::Value *serverMessage = waitForMsg(net::MSG::STATUS); // receiving server response
	JSON::Dict const & received = DICT(serverMessage);
	if (ISSTR(received.get("data"))
		&& STR(received.get("data")).value() == net::MSG::USER_EXISTS){
		throw UserAlreadyExistsException();
	}
}

void CLI::registerUser(string username, string passwd)
{
	JSON::Dict toSend, received, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = waitForMsg(net::MSG::STATUS);	// receiving server response

	if (ISSTR(received.get("data"))) {
		if (STR(received.get("data")).value() == net::MSG::USER_EXISTS)
			throw UserAlreadyExistsException();
	}
	delete serverMessage;
}

vector<Installation> CLI::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = waitForMsg(net::MSG::INSTALLATIONS_LIST);
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

bool CLI::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = waitForMsg(net::MSG::INSTALLATION_UPGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
	}
	delete serverResponse;
	return ret;
}

bool CLI::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = waitForMsg(net::MSG::INSTALLATION_DOWNGRADE);
	JSON::Dict const & received = DICT(serverResponse);
	
	if (ISBOOL(received.get("data")))
	{
		ret = received.get("data");
	}
	
	delete serverResponse;
	return ret;
}

vector<std::string> CLI::getConnectedUsersList(){// TODO
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = waitForMsg(net::MSG::CONNECTED_USERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	if (ISLIST(received.get("data"))) {
		JSON::List & connectedUsers = LIST(received.get("data"));
		for (size_t i = 0; i<connectedUsers.len(); ++i)
			res.push_back(STR(connectedUsers[i]));
	}
	return res;
}

///////// END NEW

std::vector<Sale> CLI::updatePlayersOnSale(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	_connection.send(query);
	JSON::Value *serverResponse = waitForMsg(net::MSG::PLAYERS_ON_MARKET_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	std::vector<Sale> res;
	
	if (ISLIST(received.get("data"))){
		JSON::List & sales = LIST(received.get("data"));
		for(size_t i = 0; i<sales.len();++i)
			res.push_back(Sale(DICT(sales[i]), Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)))));
	}
	return res; 
}

void CLI::bidOnPlayer(int player_id,std::string username, int value){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	query.set("data", data);
	_connection.send(query);
	JSON::Value *serverResponse = waitForMsg(net::MSG::BID_ON_PLAYER_QUERY);
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

void CLI::addPlayerOnMarket(int player_id,std::string username, int value){
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	_connection.send(query);
	JSON::Value *serverResponse = waitForMsg(net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	JSON::Dict const & received = DICT(serverResponse);
		if(ISSTR(received.get("data"))){
			std::string res = STR((received.get("data"))).value();
			if(res == net::MSG::PLAYER_ALREADY_ON_MARKET)
				throw playerAlreadyOnMarketException();
		}
}

std::vector<Player> CLI::getPlayers(std::string username){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, username);//modif
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	_connection.send(query);
	JSON::Value *serverResponse = waitForMsg(net::MSG::PLAYERS_LIST);
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


JSON::Value* CLI::waitForMsg(std::string typeToWait)
{
	_isWaitingForMessage = true;
	JSON::Value* msg = NULL, *res = NULL;
	while (msg == NULL || _connection.available())
	{
		msg = _connection.pop();
		JSON::Dict const & dict = DICT(msg);
		if (STR(dict.get("type")).value() == typeToWait)
		{
			res = msg;
		}
		else
		{
			_messages.push(msg);
		}
	}
	_isWaitingForMessage = false;
	return res;
}

void CLI::displayNotificationsCount(){
	cout << "You have " << _messages.size() << " notifications." << endl;
}

void CLI::updateNotifications(){
	while (!_isWaitingForMessage && _connection.available()){
		_messages.push(_connection.pop());
	}
}

void CLI::handleNotification(JSON::Value *notification){
	JSON::Dict message = DICT(notification);
	string messageType;
	if (ISSTR(message.get("type")))
		messageType = STR(message.get("type")).value();
	if (messageType == net::MSG::FRIENDLY_GAME_INVITATION)
		handleFriendlyGameInvitation(message);
}

void CLI::handleFriendlyGameInvitation(JSON::Dict &message){
	if (ISSTR(message.get("data"))){
		Menu _menu;
		_menu.addToDisplay("   - accept\n");
		_menu.addToDisplay("   - deny\n");
		int option;
		bool chosen = false;
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

void CLI::acceptInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT);
	toSend.set("data", data);
	_connection.send(toSend);
}

void CLI::denyInvitationFromUser(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
	JSON::Dict data;
	data.set("username", username);
	data.set("answer", net::MSG::FRIENDLY_GAME_INVITATION_DENY);
	toSend.set("data", data);
	_connection.send(toSend);
}
