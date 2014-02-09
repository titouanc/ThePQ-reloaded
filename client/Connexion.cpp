#include <iostream>
#include <string>
#include "../common/Constants.hpp"
#include "Connexion.hpp"

using namespace std;


string askForUserData(string prompt){
	string data;
	cout << prompt;
	cin >> data;
	return data;
}

void registerUser(){
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

bool userWantsToRegister(){
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

string askForNewPassword(){
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

void login(){
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
