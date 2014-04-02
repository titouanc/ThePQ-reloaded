#include "UserController.hpp"
#include <Constants.hpp>
#include <unistd.h>

/// Constructor
UserController::UserController(ClientController const & parent) : ClientController(parent)
{
}

/**
  * Method handling user login
  * @param string : username for the account
  * @param string : password for the account
  */
void UserController::loginUser(std::string username, std::string password)
{
	JSON::Dict credentials = {
		{ net::MSG::USERNAME, JSON::String(username) },
		{ net::MSG::PASSWORD, JSON::String(password) }
	};
	say (net::MSG::LOGIN, credentials);
}

/**
  * Method handling user logout from the server
  */
void UserController::logoutUser()
{
	say (net::MSG::DISCONNECT, JSON::String(""));
	user().logout();
}

/**
  * Method handling team credentials setup for current user
  * @param string : username for which to setup the credentials
  * @param string : team credentials for current user
  */
void UserController::chooseTeamName(std::string username, std::string teamname)
{
	JSON::Dict data = {
		{ net::MSG::TEAMNAME, JSON::String(teamname) },
		{ net::MSG::USERNAME, JSON::String(username) }
	};
	say (net::MSG::USER_CHOOSE_TEAMNAME, data);
}

/**
  * Method handling a users registration to the database
  * @param string : username of the account
  * @param string : password of the account
  */
void UserController::registerUser(std::string username, std::string password)
{
	JSON::Dict credentials = {
		{ net::MSG::USERNAME, JSON::String(username) },
		{ net::MSG::PASSWORD, JSON::String(password) }
	};
	say (net::MSG::REGISTER, credentials);
}

/**
  * Method checking for duplicate user 
  * @param string : username to check against the database
  */
void UserController::doesUserExist(std::string username)
{
	say(net::MSG::USER_EXISTS, JSON::String(username));
}

/**
  * Method handling server queries
  * @param string : type of the query
  * @param JSON::Value : query to be handled
  */
void UserController::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (! ISSTR(data))
		return;
	std::string const & response = STR(data);

	if (type == net::MSG::REGISTER){
		if (response == net::MSG::USER_REGISTERED)
			onRegisterUserOK();
		else if (response == net::MSG::USER_EXISTS)
			onRegisterUserError("This username already exists. Please chose another one");
	}

	else if (type == net::MSG::TEAMNAME){
		if (response == net::MSG::TEAMNAME_REGISTERED)
			onTeamNameOK();
		else if (response == net::MSG::TEAMNAME_ERROR)
			onTeamNameError("This teamname is not available, chose another one.");
	}

	else if (type == net::MSG::LOGIN){
		if (response == net::MSG::USER_LOGIN)
			onLoginOK();
		else if (response == net::MSG::USER_CHOOSE_TEAMNAME)
			onAskTeamName();
		else {
			std::string reason = "Unknow error";
			if (response == net::MSG::PASSWORD_ERROR)
				reason = "Wrong password";
			else if (response == net::MSG::USER_NOT_FOUND)
				reason = "User not found";
			else if (response == net::MSG::ALREADY_LOGGED_IN)
				reason = "Already logged in from another location";

			onLoginError(reason);
		}
	}
}