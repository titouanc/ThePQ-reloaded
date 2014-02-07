#include <iostream>
#include <string>
#include "Messages.hpp"
#include "Register.hpp"
#include "../common/Constants.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	printLaunchingMessage();

	// TODO add an option for the user to quit before logging in/registering

	UserStatus userStatus = UserNotFound;
	while (userStatus != UserLoggedIn){
		string username;
		cout << "Username : " ;
		cin >> username;
		// TODO send username to server
		// TODO receive username from server
		// TODO receive userStatus from server 

		if (userStatus == UserRegistered){
			string password;
			cout << "Password : ";
			cin >> password;
			// TODO hash password
			// TODO send password hash to server
			bool isPasswordCorrect = false;
			// TODO receive isPasswordCorrect from server
			if (isPasswordCorrect){
				// TODO receive userStatus from server,
				// normally UserLoggedIn but let's handle exceptions if not.
			}
			else{
				cout << "Sorry, wrong password. Try again." << endl;
			}
		}

		else if (userStatus == UserNotFound){
			cout << "User " << username << " was not found." << endl;
			if (userWantsToRegister()){
				string password = askForNewPassword();
				// TODO hash password
				// TODO send hashed password to server
				// TODO received hashed password from server
				// TODO send isPasswordCorrect to server
				// TODO receive userStatus from server
			}
		}
	}
	
	return 0;
}