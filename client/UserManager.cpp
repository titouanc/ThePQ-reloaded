#include "UserManager.hpp"

UserManager::UserManager(net::ClientConnectionManager& connection) : 
	_connection(connection)
{
}


void UserManager::loginMenu()
{
	/* user menu */
	Menu _menu;
	_menu.addToDisplay("   - login\n");
	_menu.addToDisplay("   - register\n");
	_menu.addToDisplay("   - quit\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				doLogin();
				break;
			case 2:
				doRegister();
				break;
			default:
				break;
		}
	}
	while (option != 3);
}

void UserManager::doLogin()
{	
	string username = Menu::askForUserData("Username : ");
	string password = Menu::askForUserData("Password : ");
	
	try {
		cout << "Please wait..." << endl;
		doLoginUser(username, password);
		_username = username;
		cout << "You have successfully logged in! Welcome! :)\n\n\n" << endl;
		//~ mainMenu();
	}
	catch (UserNotFoundException e)
	{
		cout << "\nUser not found" << endl;
	}
	catch (WrongPasswordException e)
	{
		cout << "\nWrong password" << endl;
	}
}

void UserManager::doRegister()
{
	bool registered = false;
	for (int i = 0; i < 3 && ! registered; ++i)
	{
		string username = Menu::askForUserData("Pick a username : ");
		try {
			cout << "Please wait..." << endl;
			doesUserExist(username);
			string password = "a";
			string passwordConfirmation;
			while (password != passwordConfirmation){
				password = Menu::askForUserData("Enter a new password : ");
				passwordConfirmation = Menu::askForUserData("Confirm password : ");
				if (password != passwordConfirmation)
					cout << "The two passwords entered were not the same." << endl;
			}
			cout << "Please wait..." << endl;
			doRegisterUser(username, password);
			registered = true;
			cout << "You have successfully registered! You can now login." << endl;
		}
		catch (UserAlreadyExistsException e) {
			cout << "User name already exists. Try again with a different username." << endl;		
		}
	}
}

void UserManager::doLoginUser(std::string username, std::string password)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS);
	JSON::Dict const & received = DICT(serverMessage); 	// receiving server response
	if (ISSTR(received.get("data"))) {
		if (STR(received.get("data")).value() == net::MSG::PASSWORD_ERROR)
		{
			delete serverMessage;
			throw WrongPasswordException();
		}
		else if (STR(received.get("data")).value() == net::MSG::USER_NOT_FOUND)
		{
			delete serverMessage;
			throw UserNotFoundException();
		}
	}
	delete serverMessage;
}

void UserManager::doRegisterUser(std::string username, std::string password)
{
	JSON::Dict toSend, received, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS);	// receiving server response

	if (ISSTR(received.get("data"))) {
		if (STR(received.get("data")).value() == net::MSG::USER_EXISTS)
			throw UserAlreadyExistsException();
	}
	delete serverMessage;
}

void UserManager::doesUserExist(std::string username)
{
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS);
	toSend.set("data", username);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS); // receiving server response
	JSON::Dict const & received = DICT(serverMessage);
	if (ISSTR(received.get("data"))
		&& STR(received.get("data")).value() == net::MSG::USER_EXISTS){
		throw UserAlreadyExistsException();
	}
}
