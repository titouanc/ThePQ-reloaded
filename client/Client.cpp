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
				Connection::login();
			} catch (WrongPasswordException &wpe){
				cout << "Wrong password. Please try again." << endl;
			} catch (UserNotFoundException &unfe){
				cout << "User was not found." << endl;
				if (Connection::userWantsToRegister())
					try{
						Connection::registerUser();
					} catch (UserAlreadyExistsException &uaee){
						cout << "User already exists. Please try again with another username." << endl;
					}
			}
		}

		// registering
		else if (_userChoice == "r" || _userChoice == "R"){
			try{
				Connection::registerUser();
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
