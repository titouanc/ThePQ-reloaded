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
	Message::printLaunchingMessage();

	// -------------------------------------------------------------------------
	// LOGIN + REGISTERING LOOP

	Message::printLoggedOffActions();
	cin >> _userChoice;

	while (_userChoice != "q" && _userChoice != "Q" && ! Connection::isLogged()){

		// login
		if (_userChoice == "l" || _userChoice == "L"){
			try{
				Client::login();
			} catch (WrongPasswordException &wpe){
				cout << "Wrong password. Please try again." << endl;
			} catch (UserNotFoundException &unfe){
				cout << "User was not found." << endl;
				if (Client::userWantsToRegister())
					try{
						Client::registerUser();
					} catch (UserAlreadyExistsException &uaee){
						cout << "User already exists. Please try again with another username." << endl;
					}
			}
		}

		// registering
		else if (_userChoice == "r" || _userChoice == "R"){
			try{
				Client::registerUser();
			} catch (UserAlreadyExistsException &uaee){
				cout << "User already exists. Please try again with another username." << endl;
			}
		}

		// input not understood
		else {
			cout << "Please enter a valid response." << endl;
			Message::printLoggedOffActions();
		}
		if (!Connection::isLogged()){
			Message::printLoggedOffActions();
			cin >> _userChoice;
		}
	}

	// -------------------------------------------------------------------------
	// MAIN MENU LOOP

	Message::printMainMenu();
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
			Message::printLoggedOffActions();
		}
	}


	Message::printGoodbyeMessage();
}

string Client::askForUserData(string prompt){
	string data;
	cout << prompt;
	cin >> data;
	return data;
}

void Client::registerUser(){
	UserStatus userStatus = UserNotFound;	// handy initialization
	string username = askForUserData("Pick a username : ");
	// TODO send username to server
	// TODO receive userStatus from server
	
	if (userStatus == UserNotFound){
		string password = askForNewPassword();
		// TODO hash password
		// TODO send hashed password to server
		// TODO receive hashed password from server
		cout << "You have successfully registered! You can now login." << endl;
	}
	else throw UserAlreadyExistsException();
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

void Client::login(){
	UserStatus userStatus = UserNotFound;	// handy initialization
	
	string username = askForUserData("Username : ");
	// TODO send username to server
	// TODO receive username from server
	// TODO receive userStatus from server 

	if (userStatus == UserRegistered){
		string password = askForUserData("Password : ");
		// TODO hash password
		// TODO send password hash to server
		bool isPasswordCorrect = false;
		// TODO receive isPasswordCorrect from server
		if (isPasswordCorrect){
			// TODO receive userStatus from server

			/*
			// TODO we need to implement this only if we want the server to send
			// a confirmation after saying that the password was correct.
			if (userStatus != UserLoggedIn){
				// TODO throw some kind of exception
			}
			*/
			cout << "You have successfully logged in! Welcome! :)" << endl;
		}
		else throw WrongPasswordException();
	}
	else throw UserNotFoundException();
}
