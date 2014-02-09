#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "Message.hpp"
#include "Connection.hpp"
#include "Exception.hpp"
#include "../common/Constants.hpp"


class Client
{
public:
	Client();
	~Client();
	
	void run();

private:
	std::string _userChoice;
	Connection _connection;
	
	// Registering
	static void registerUser();
	static bool userWantsToRegister(); // asks the user if he wants to register when login has
								// failed because the username specified doesn't exist.
	static std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.

	// Logging in
	static void login();
	static std::string askForUserData(std::string prompt); // returns the user input.
};

#endif // __CLIENT_HPP
