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
												_isRunning(true)
{
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
			mainMenu();
		}
		else
		{
			_isRunning = showUserMenu();
		}
	}
	cout << goodBye();
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
				_marketManager.showMenu();
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
				_stadiumManager.showMenu();
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

/* Market menu */


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
			cout << "Username not found" << endl;
		}
		else {
			cout << STR(DICT(received.get("data")).get("username")).value();
			cout << " denied your invitation. Sorry!" << endl;
		}
	}
}

void Client::printConnectedUsersList(){
	vector<std::string> connectedUsers = getConnectedUsersList();
	cout << "Here are all the connected users : " << endl;
 	for (size_t i=0; i < connectedUsers.size(); ++i)
 		cout << "  - " << connectedUsers[i] << endl;
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
			cout << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " was not sold. :(" << endl; 
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
		_user.login(username);
		//~ mainMenu();
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
				printPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 1);
}

void Client::printPlayers(){
	_teamManager.loadPlayers();
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_user.players.size();++i){
		cout << _user.players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}
