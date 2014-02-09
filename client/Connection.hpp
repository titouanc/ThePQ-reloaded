#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include "net.hpp"
#include "json.h"
#include "Exception.hpp"
#include "../common/Constants.hpp"

class Connection
{
public:

	// Registering
	static void registerUser();
	static bool userWantsToRegister(); // asks the user if he wants to register when login has
								// failed because the username specified doesn't exist.
	static std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.

	// Logging in
	static void login();
	
	static bool isLogged();
	
private:
	static bool _isLogged;
	
	static std::string askForUserData(std::string prompt); // returns the user input.
};

#endif
