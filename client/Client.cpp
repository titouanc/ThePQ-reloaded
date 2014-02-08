#include <iostream>
#include <string>
#include "Messages.hpp"
#include "Connexion.hpp"
#include "../common/Constants.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	printLaunchingMessage();

	// -------------------------------------------------------------------------
	// LOGIN + REGISTERING LOOP

	bool isLoggedIn = false;

	printLoggedOffActions();
	string userChoice;
	cin >> userChoice;

	while (userChoice != "q" && userChoice != "Q" && isLoggedIn == false){

		// login
		if (userChoice == "l" || userChoice == "L"){
			try{
				login();
				isLoggedIn = true;
			} catch (WrongPasswordException &wpe){
				cout << "Wrong password. Please try again." << endl;
			} catch (UserNotFoundException &unfe){
				cout << "User was not found." << endl;
				if (userWantsToRegister())
					try{
						registerUser();
					} catch (UserAlreadyExistsException &uaee){
						cout << "User already exists. Please try again with another username." << endl;
					}
			}
		}

		// registering
		else if (userChoice == "r" || userChoice == "R"){
			try{
				registerUser();
			} catch (UserAlreadyExistsException &uaee){
				cout << "User already exists. Please try again with another username." << endl;
			}
		}

		// input not understood
		else {
			cout << "Please enter a valid response." << endl;
			printLoggedOffActions();
		}
		if (!isLoggedIn){
			printLoggedOffActions();
			cin >> userChoice;
		}
	}

	// -------------------------------------------------------------------------
	// MAIN MENU LOOP

	printMainMenu();
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
			printLoggedOffActions();
		}
	}


	printGoodbyeMessage();
	return 0;
}