#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

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

string askForUserData(string prompt);

// Registering
void registerUser();
bool userWantsToRegister();
string askForNewPassword();

// Logging in
void login();
