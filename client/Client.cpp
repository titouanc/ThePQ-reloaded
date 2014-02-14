#include "Client.hpp"

using namespace std;

string Client::_userChoice;
Connection Client::_connection;
string Client::_prompt = " > ";

void Client::run()
{
	cout << Message::splashScreen();

	/* user menu */
	Menu loginMenu;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ogin\n";
	message+= "   - (r)egister\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	loginMenu.setMessage(message);
	loginMenu.addOption('l', login);
	loginMenu.addOption('r', registerUser);
	loginMenu.run();

	cout << Message::goodBye();
}

void Client::login(){
	
	string username = askForUserData("Username : ");
	string password = askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		_connection.loginUser(username, password);
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

void Client::registerUser(){
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
void Client::mainMenu()
{
	Menu main;
	string message;
	message+= "You can : \n";
	message+= "   - (m)anage your team and stadium\n";
	message+= "   - (p)lay a friendly game\n";
	message+= "   - (q)uit\n";
	message+= _prompt;
	main.setMessage(message);
	main.addOption('m', managementMenu);
	main.addOption('p', friendlyMatchMenu);
	main.run();
}

/* Management menu */
void Client::managementMenu()
{
	Menu mgt;
	string message;
	message+= "You can : \n";
	message+= "   - manage your (s)tadium and installations\n";
	message+= "   - manage your (p)layers\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	mgt.setMessage(message);
	mgt.addOption('s', stadiumMenu);
	mgt.addOption('p', playersMenu);
	mgt.run();
}

void Client::stadiumMenu()
{
	StadiumManager::setConnection(&_connection);
	StadiumManager::loadInstallations();
	Menu stadium;
	string message;
	message+= "You can : \n";
	message+= "    - (v)iew your installations\n";
	message+= "    - (q)uit to management menu\n";
	stadium.addOption('v', StadiumManager::printInstallationsList);
	stadium.setMessage(message);
	// TODO : stadium menu
	stadium.run();
}

void Client::playersMenu()
{
	Menu players;
	string message;
	message+= "You can : \n";
	message+= "    - (q)uit to management menu\n";
	players.setMessage(message);
	// TODO : players menu
	players.run();
}

/* Friendly match menu */
void Client::friendlyMatchMenu()
{
	Menu friendly;
	string message;
	message+= "You can : \n";
	message+= "   - (l)ist all connected players\n";
	message+= "   - (c)hoose one to play a friendly game with\n";
	message+= "   - (q)uit to main menu\n";
	message+= _prompt;
	friendly.setMessage(message);
	friendly.addOption('l', listUsers);
	friendly.addOption('c', chooseUser);
	friendly.run();
}

void Client::listUsers()
{
	FriendlyGameManager::loadConnectedUsersList(&_connection);
	FriendlyGameManager::printConnectedUsersList();
}

void Client::chooseUser()
{
	// TODO : choose user for friendly match
}

/* Private methods */

string Client::askForUserData(string prompt){
	string data;
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
