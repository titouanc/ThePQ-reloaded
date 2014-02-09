#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include <iostream>
#include <string>
#include "net.hpp"
#include "json.h"
#include "Message.hpp"
#include "Exception.hpp"

using namespace std;

class Connection
{
public:

	// Registering
	static void registerUser();
	static bool userWantsToRegister(); // asks the user if he wants to register when login has
								// failed because the username specified doesn't exist.
	static string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.

	// Logging in
	static void login();
	
	static bool isLogged();
	
private:
	static bool _isLogged;
	
	static string askForUserData(string prompt); // returns the user input.
};

#endif
