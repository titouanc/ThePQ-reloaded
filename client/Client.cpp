#include "Client.hpp"

using namespace std;

Client::Client()
{
	
}

Client::~Client()
{
	
}

void Client::run()
{
	cout << Message::printLaunchingMessage();

	// -------------------------------------------------------------------------
	// LOGIN + REGISTERING LOOP

	cout << Message::printLoggedOffActions();
	cin >> _userChoice;

	while (_userChoice != "q" && _userChoice != "Q" && ! _connection.isLogged()){

		// login
		if (_userChoice == "l" || _userChoice == "L"){
			login();
			if (_connection.isLogged() == false && userWantsToRegister() == true)
			{
				registerUser();
			}
		}

		// registering
		else if (_userChoice == "r" || _userChoice == "R"){
			registerUser();
		}

		// input not understood
		else {
			cout << "Please enter a valid response." << endl;
			cout << Message::printLoggedOffActions();
		}
		if (!_connection.isLogged()){
			cout << Message::printLoggedOffActions();
			cin >> _userChoice;
		}
		else
		{
			// -------------------------------------------------------------------------
			// MAIN MENU LOOP

			cout << Message::printMainMenu();
			cin >> _userChoice;

			while (_userChoice != "q" && _userChoice != "Q"){

				// team and stadium management
				if (_userChoice == "m" || _userChoice == "M"){
					// managementMenu();
				}

				// play a friendly game
				else if (_userChoice == "p" || _userChoice == "P"){
					// friendlyGameLobby();
				}

				// input not understood
				else {
					cout << "Please enter a valid response." << endl;
					cout << Message::printLoggedOffActions();
				}
			}
		}
	}

	cout << Message::printGoodbyeMessage();
}

void Client::login(){
	
	string username = askForUserData("Username : ");
	// TODO send username to server
	// TODO receive username from server
	// TODO receive userStatus from server 
	string password = askForUserData("Password : ");
	
	try {
		_connection.loginUser(username, password);
		cout << "You have successfully logged in! Welcome! :)" << endl;
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
	// TODO send username to server
	// TODO receive userStatus from server
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

string Client::askForUserData(string prompt){
	string data;
	cout << prompt;
	cin >> data;
	return data;
}

bool Client::userWantsToRegister(){
	bool done = false;
	bool res = false;
	while (!done){
		string wantsToRegister = askForUserData("Do you want to register? (y/n) : ");
		if (wantsToRegister == "y" || wantsToRegister == "Y") {
			res = true;
			done = true;
		}
		else if (wantsToRegister == "n" || wantsToRegister == "N") {
			res = false;
			done = true;
		}
		else
			cout << "Please enter a valid response." << endl;
	}
	return res;
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
