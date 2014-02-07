#include <iostream>
#include <string>
#include "Register.hpp"

using namespace std;

bool userWantsToRegister(){
	bool done = false;
	bool res = false;
	while (!done){
		cout << "Do you want to register? (y/n)" << endl;
		string wantsToRegister;
		cin >> wantsToRegister;
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
		cout << "Enter a new password : ";
		cin >> password;
		cout << "Confirm password : ";
		cin >> passwordConfirmation;
		if (password != passwordConfirmation)
			cout << "The two passwords entered were not the same." << endl;
	}
	return password;
}