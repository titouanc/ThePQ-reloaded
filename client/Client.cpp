#include <iostream>
#include <string>
#include "Messages.hpp"
#include "Connexion.hpp"
#include "../common/Constants.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	printLaunchingMessage();

	printLoggedOffActions();
	string userChoice;
	cin >> userChoice;

	while (userChoice != "q" && userChoice != "Q"){
		if (userChoice == "l" || userChoice == "L"){
			try{
				login();
			} catch (WrongPasswordException &wpe){
				cout << "Wrong password. Please try again." << endl;
			} catch (UserNotFoundException &unfe){
				cout << "User was not found." << endl;
				if (userWantsToRegister())
					registerUser();
			}
		}
		else if (userChoice == "r" || userChoice == "R"){
			try{
				registerUser();
			} catch (UserAlreadyExistsException &uaee){
				cout << "User already exists. Please try again with another username." << endl;
			}
		}
		else {
			cout << "Please enter a valid response." << endl;
			printLoggedOffActions();
		}
		printLoggedOffActions();
		cin >> userChoice;
	}

	printGoodbyeMessage();
	return 0;
}