#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <string>
#include "Message.hpp"
#include "Connection.hpp"
#include "Exception.hpp"
#include "Menu.hpp"


class Client
{
public:
	static void run();

private:
	Client()
	{}
	static std::string _userChoice;
	static Connection _connection;
	static std::string _prompt;
	
	// User menu
	static void registerUser();
	static void login();
	
	// Menus
	static void mainMenu();
	static void managmentMenu();
	static void stadiumMenu();
	static void playersMenu();
	static void friendlyMatchMenu();
	static void listUsers();
	static void chooseUser();
	
	// utils
	static std::string askForUserData(std::string prompt); // returns the user input.
	static std::string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.
};

#endif // __CLIENT_HPP
