#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include "net.hpp"
#include "json.h"
#include "Message.hpp"

using namespace std;


string askForUserData(string prompt); // returns the user input.

// Registering
void registerUser();
bool userWantsToRegister(); // asks the user if he wants to register when login has
							// failed because the username specified doesn't exist.
string askForNewPassword(); // prompts the user to create a new password with 
							// confirmation. returns the password.

// Logging in
void login();


// Exceptions
class WrongPasswordException : public runtime_error{
public:
	WrongPasswordException():runtime_error("Wrong password"){};
};
class UserNotFoundException : public runtime_error{
public:
	UserNotFoundException():runtime_error("User not found"){};
};
class UserAlreadyExistsException : public runtime_error{
public:
	UserAlreadyExistsException():runtime_error("User already exists"){};
};

#endif
