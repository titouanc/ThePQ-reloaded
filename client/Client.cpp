#include "Client.hpp"
#include "Menu.hpp"
using namespace std;

std::string Client::_userChoice;
Connection Client::_connection;

void Client::run()
{
	cout << Message::splashScreen();

	Menu loginMenu;
	loginMenu.setMessage(Message::loginMenu());
	loginMenu.addOption("l", Client::login);
	loginMenu.addOption("L", Client::login);
	loginMenu.addOption("r", Client::registerUser);
	loginMenu.addOption("R", Client::registerUser);
	loginMenu.run();

	cout << Message::goodBye();
}

void Client::login(){
	
	string username = askForUserData("Username : ");
	string password = askForUserData("Password : ");
	
	try {
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
	string username = askForUserData("Pick a username : ");
	string password = askForNewPassword();
	
	try {
		_connection.registerUser(username, password);
		cout << "You have successfully registered! You can now login." << endl;
	}
	catch (UserAlreadyExistsException e)
	{
		cout << "User name already exists" << endl;
	}
}

void Client::mainMenu()
{
	Menu main;
	main.setMessage(Message::mainMenu());
	main.addOption("m", managmentMenu);
	main.addOption("M", managmentMenu);
	main.addOption("p", friendlyMatchMenu);
	main.addOption("P", friendlyMatchMenu);
	main.run();
}

void Client::managmentMenu()
{
	Menu mgt;
	// TODO ; create managment menu
	mgt.run();
}

void Client::friendlyMatchMenu()
{
	Menu friendly;
	// TODO : create friendly match menu
	friendly.run();
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
