#include <iostream>
#include <string>
#include "Messages.hpp"
#include "Register.hpp"
#include "../common/Constants.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
	printLaunchingMessage();

	UserStatus userStatus = UserNotFound;
	while (userStatus != UserLoggedIn){
		string username;
		cout << "Username : " ;
		cin >> username;
		// TODO send username to server
		// TODO receive username from server
		// TODO receive userStatus from server 

		if (userStatus != UserNotFound)
			userStatus = UserNotFound;
		else
			userStatus = UserRegistered;

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
				// Creating a new password
				cout << "you want to register" << endl;
			}
			else {
				cout << "you don't want to register" << endl;
			}
			
		}
	}
	
	return 0;
}