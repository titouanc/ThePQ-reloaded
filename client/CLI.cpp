#include "CLI.hpp"

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
										_prompt(">")
{
	
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
	Menu loginMenu;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ogin\n";
	message+= "   - (r)egister\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	loginMenu.setMessage(message);
	loginMenu.addOption('l', new ClassCallback<CLI>(this,&CLI::login));
	loginMenu.addOption('r', new ClassCallback<CLI>(this,&CLI::registerUser));

	bool error = false;
	do {
		try{ 
			error = false;
			loginMenu.run(); 
		} catch (std::runtime_error &err){
			error = true;
			cerr << "\033[31mError " << humanExcName(typeid(err).name()) 
				     << "\t" << err.what() << "\033[0m" << endl;
		} catch (...){
			error = true;
			cerr << "\033[31mUnknow error "
				 << "\033[0m" << endl;
		}
	} while (error);
}

void CLI::login(){
	
	string username = askForUserData("Username : ");
	string password = askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		loginUser(username, password);
		cout << "You have successfully logged in! Welcome! :)" << endl;
		mainMenu();
	}
	catch (UserNotFoundException e)
	{
		cout << "User not found" << endl;
	}
	catch (WrongPasswordException e)
	{
		cout << "Wrong password" << endl;
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
	Menu main;
	string message;
	message+= "You can : \n";
	message+= "   - (m)anage your team and stadium\n";
	message+= "   - (a)ccess market\n";
	message+= "   - (p)lay a friendly game\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	main.setMessage(message);
	main.addOption('m', new ClassCallback<CLI>(this, &CLI::managementMenu));
	main.addOption('a', new ClassCallback<CLI>(this, &CLI::marketMenu));
	main.addOption('p', new ClassCallback<CLI>(this, &CLI::friendlyMatchMenu));
	main.run();
}

/* Management menu */
void CLI::managementMenu()
{
	Menu mgt;
	string message;
	message+= "You can : \n";
	message+= "   - manage your (s)tadium and installations\n";
	message+= "   - manage your (p)layers\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	mgt.setMessage(message);
	mgt.addOption('s', new ClassCallback<CLI>(this, &CLI::stadiumMenu));
	mgt.addOption('p', new ClassCallback<CLI>(this, &CLI::playersMenu));
	mgt.run();
}

void CLI::stadiumMenu()
{
	Menu stadium;
	string message;
	message+= "You can : \n";
	message+= "    - (v)iew your installations\n";
	message+= "    - (u)pgrade an installation\n";
	message+= "    - (d)owngrade an installation\n";
	message+= "    - (q)uit to management menu\n";
	stadium.addOption('v', new ClassCallback<CLI>(this, &CLI::printInstallationsList));
	stadium.addOption('u', new ClassCallback<CLI>(this, &CLI::upgradeInstallation));
	stadium.addOption('d', new ClassCallback<CLI>(this, &CLI::downgradeInstallation));
	stadium.setMessage(message);
	// TODO : stadium menu
	stadium.run();
}

void CLI::playersMenu()
{
	Menu players;
	string message;
	message+= "You can : \n";
	message+= "    - (q)uit to management menu\n";
	players.setMessage(message);
	// TODO : players menu
	players.run();
}

/* Market menu */
void CLI::marketMenu(){
	Menu market;
	string message;
	message+="You can : \n";
	message+="   - (p)ut a player on sale\n";
	message+="   - (s)ee the players on sale\n";
	message+="   - (q)uit to main menu\n";
	message+= _prompt;
	market.setMessage(message);
	market.addOption('p', new ClassCallback<CLI>(this, &CLI::printPlayersOnSale));
	market.addOption('s', new ClassCallback<CLI>(this, &CLI::salePlayer));
	market.run();
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
			cout << bidValue << "is not between " << range[0] << " and " << range[1] << " !\nTry again :" << endl;
			cout << _prompt;
			cin >> bidValue;
		}
		try{
		addPlayerOnMarket(player_id, _team_id, bidValue);
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
		JSON::Dict & sale = _playersOnSale[i];
		Player player(DICT(sale.get(net::MSG::PLAYER)));
		cout << i << " - " << STR(sale.get(net::MSG::PLAYER_ID)) << endl;
		cout << "Time left (in seconds) : " << 	STR(sale.get(net::MSG::BID_TIMER)) 	<< endl;
		cout << "Bid value 				: " <<	STR(sale.get(net::MSG::BID_VALUE)) 	<< endl;
		cout << "Next bid value 		: "	<< 	STR(sale.get(net::MSG::NEXT_BID)) 	<< endl;
		cout << "Player ID 				: "	<<	STR(sale.get(net::MSG::PLAYER_ID))	<< endl;
		//cout << "Player infos			: "	<< 	player 								<< endl; TODO << FOR PLAYER
		cout << "--------------------------------" << endl;
	}
	cout << "=================================================" << endl;
	Menu marketList;
	string message;
	message+="You can : \n";
	message+="   - (p)lace a bid on a player\n";
	message+="   - (q)uit to market menu\n";
	message+= _prompt;
	marketList.setMessage(message);
	marketList.addOption('p', new ClassCallback<CLI>(this, &CLI::placeBid));
	marketList.run();
}

void CLI::placeBid(){
	int player_id, value;
	string response;
	cout << "Enter the ID of the player you wish to bid on : " << endl;
	cout << _prompt;
	cin >> player_id;
	for(size_t i = 0; i<_playersOnSale.size();++i){		//Getting the next bid value (which is in the JSON::Dict sent by server)
		if(player_id == INT(_playersOnSale[i].get(net::MSG::PLAYER_ID))){
			value = INT(_playersOnSale[i].get(net::MSG::NEXT_BID));
		}
	}
	try{
		bidOnPlayer(player_id, _team_id, value);
		cout << "Bid successfully placed ! Hurra !" << endl;
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

void CLI::printPlayers(){
	_players = getPlayers(_team_id);
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_players.size();++i){
//		cout << _players[i] << endl; TODO << FOR PLAYER
		cout << "--------------------------------------" << endl;
	}
	cout << "==============================================" << endl;
}

/* Friendly match menu */
void CLI::friendlyMatchMenu()
{
	Menu friendly;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ist all connected players\n";
	message+= "   - (c)hoose one to play a friendly game with\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	friendly.setMessage(message);
	friendly.addOption('l', new ClassCallback<CLI>(this, &CLI::printConnectedUsersList));
	friendly.addOption('c', new ClassCallback<CLI>(this, &CLI::chooseUser));
	friendly.run();
}

void CLI::chooseUser()
{
	// TODO : choose user for friendly match
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
void CLI::loginUser(string username, string passwd)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.pop();	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
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

	JSON::Value *serverMessage = _connection.pop(); // receiving server response

	if(ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data")) 
				&& STR(received.get("data")).value() == net::MSG::USER_EXISTS){
				throw UserAlreadyExistsException();
			}
		}
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

	JSON::Value *serverMessage = _connection.pop();	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == net::MSG::USER_EXISTS)
					throw UserAlreadyExistsException();
			}
		}
	}
	// User is registered
}

vector<Installation> CLI::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.pop();
	if (ISDICT(serverResponse))
	{
		JSON::Dict response = DICT(serverResponse);
		if (ISLIST(response.get("data")))
		{
			toFill = LIST(response.get("data"));
		}
	}
	vector<Installation> vec;
	for (size_t i = 0; i < toFill.len(); ++i)
	{
		vec.push_back(DICT(toFill[i]));
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
	
	JSON::Value *serverResponse = _connection.pop();
	if (ISDICT(serverResponse))
	{
		JSON::Dict const & received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_UPGRADE)
		{
			ret = received.get("data");
		}
	}
	return ret;
}

bool CLI::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.pop();
	if (ISDICT(serverResponse))
	{
		JSON::Dict const &received = DICT(serverResponse);
		
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_DOWNGRADE)
		{
			ret = received.get("data");
		}
	}
	delete serverResponse;
	return ret;
}

vector<std::string> CLI::getConnectedUsersList(){
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	_connection.send(query);

	JSON::Value *serverResponse = _connection.pop();
	if (ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & connectedUsers = LIST(received.get("data"));
			for (size_t i = 0; i<connectedUsers.len(); ++i)
				res.push_back(STR(connectedUsers[i]));
		}
	}
	return res;
}

std::vector<JSON::Dict> CLI::updatePlayersOnSale(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.pop();
	std::vector<JSON::Dict> res;
	if (ISDICT(serverResponse)) {
		JSON::Dict received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & sales = LIST(received.get("data"));
			for(size_t i = 0; i<sales.len();++i)
				res.push_back(DICT(sales[i]));
		}
	}
	return res; 
}

void CLI::bidOnPlayer(int player_id,int team_id, int value){
	JSON::Dict query, data;
	data.set(net::MSG::TEAM_ID,team_id);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	query.set("data", data);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.pop();
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISSTR(received.get("type")) && ISDICT(received.get("data"))){
			std::string res = STR(DICT(received.get("data")).get(net::MSG::SERVER_RESPONSE));
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
}

void CLI::addPlayerOnMarket(int player_id,int team_id, int value){
	JSON::Dict query, data;
	data.set(net::MSG::TEAM_ID,team_id);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.pop();
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISSTR(received.get("type")) && ISDICT(received.get("data"))){
			std::string res = STR(DICT(received.get("data")).get(net::MSG::SERVER_RESPONSE));
			if(res == net::MSG::PLAYER_ALREADY_ON_MARKET)
				throw playerAlreadyOnMarketException();
		}
	}
}

std::vector<Player> CLI::getPlayers(int team_id){
	JSON::Dict query, data;
	JSON::List toFill;
	data.set(net::MSG::TEAM_ID, team_id);
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.pop();
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			toFill = LIST(received.get("data"));
		}
	}
	std::vector<Player> myplayers;
	for(size_t i=0; i<toFill.len();++i){
		myplayers.push_back(DICT(toFill[i]));
	}
	delete serverResponse;
	return myplayers;
}
///////// END NEW
