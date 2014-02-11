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
	
	static void run();

private:
	Client();
	static std::string _userChoice;
	static Connection _connection;
	
	// User menu
	static void registerUser();
	static void login();
	
	// Menu
	static void mainMenu();
	
	// utils
	static std::string askForUserData(std::string prompt); // returns the user input.
	static std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.
};

#endif // __CLIENT_HPP
