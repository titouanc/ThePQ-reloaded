#include "Client.hpp"

Client::Client()
{
	_socket.connect("127.0.0.1", 6666);
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
	string userChoice;
	cin >> userChoice;

	while (userChoice != "q" && userChoice != "Q" && ! Connection::isLogged()){

		// login
		if (userChoice == "l" || userChoice == "L"){
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
		else if (userChoice == "r" || userChoice == "R"){
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
			cin >> userChoice;
		}
	}

	// -------------------------------------------------------------------------
	// MAIN MENU LOOP

	Message::printMainMenu();
	cin >> userChoice;

	while (userChoice != "q" && userChoice != "Q"){

		// team and stadium management
		if (userChoice == "m" || userChoice == "M"){
			// managementMenu();
		}

		// play a friendly game
		else if (userChoice == "p" || userChoice == "P"){
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
