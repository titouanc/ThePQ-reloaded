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

CLI::CLI(NetConfig const &config) : 	_prompt(">"), 
												_connection(config.host, config.port)
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
		_connection.loginUser(username, password);
		_username = username;//modif
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
			_connection.doesUserExist(username);
			string password = askForNewPassword();
			cout << "Please wait..." << endl;
			_connection.registerUser(username, password);
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
	message+= "   - (a)ccess the market\n";//modif
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
	message+= "    - (s)ee a list of your players\n";//modif
	message+= "    - (q)uit to management menu\n";
	players.addOption('s', new ClassCallback<CLI>(this, &CLI::printPlayers));//modif
	players.setMessage(message);
	// TODO : players menu
	players.run();
}
//modif
void CLI::printPlayers(){
	_players = _connection.getPlayers(_username);//modif
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_players.size();++i){
		cout << _players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
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
	market.addOption('p', new ClassCallback<CLI>(this, &CLI::salePlayer));
	market.addOption('s', new ClassCallback<CLI>(this, &CLI::printPlayersOnSale));//modif
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
			cout << bidValue << " is not between " << range[0] << " and " << range[1] << " !\nTry again :" << endl;//modif
			cout << _prompt;
			cin >> bidValue;
		}
		try{
		_connection.addPlayerOnMarket(player_id, _username, bidValue);
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
	_playersOnSale = _connection.updatePlayersOnSale();
	cout << "================ PLAYERS ON SALE ================" << endl;
	for(size_t i=0;i<_playersOnSale.size();++i){
		JSON::Dict & sale = _playersOnSale[i];
		Player player(DICT(sale.get(net::MSG::PLAYER)));
		cout << i << " - " << STR(sale.get(net::MSG::PLAYER_ID)) << endl;
		cout << "Time left (in seconds) : " << 	STR(sale.get(net::MSG::BID_TIMER)) 	<< endl;
		cout << "Bid value 				: " <<	STR(sale.get(net::MSG::BID_VALUE)) 	<< endl;
		cout << "Next bid value 		: "	<< 	STR(sale.get(net::MSG::NEXT_BID)) 	<< endl;
		cout << "Player ID 				: "	<<	STR(sale.get(net::MSG::PLAYER_ID))	<< endl;
		cout << "Player infos			: "	<< 	player 								<< endl;
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
		_connection.bidOnPlayer(player_id, _username, value);
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
	_installations = _connection.getInstallationsList();
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
		if (_connection.upgradeInstallation(choice))
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
		if (_connection.downgradeInstallation(choice))
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
	vector<std::string> connectedUsers = _connection.getConnectedUsersList();
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
