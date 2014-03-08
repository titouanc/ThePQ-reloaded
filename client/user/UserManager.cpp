#include "UserManager.hpp"
#include <Constants.hpp>

UserManager::UserManager(ClientManager const & parent) : ClientManager(parent)
{
}

void UserManager::loginUser(std::string username, std::string password)
{
	JSON::Dict credentials = {
		{ net::MSG::USERNAME, JSON::String(username) },
		{ net::MSG::PASSWORD, JSON::String(password) }
	};
	say (net::MSG::LOGIN, credentials);
}

void UserManager::logoutUser()
{
	say (net::MSG::DISCONNECT, JSON::String(""));
	user().logout();
}

void UserManager::chooseTeamName(std::string username, std::string teamname)
{
	JSON::Dict data = {
		{ net::MSG::TEAMNAME, JSON::String(teamname) },
		{ net::MSG::USERNAME, JSON::String(username) }
	};
	say (net::MSG::USER_CHOOSE_TEAMNAME, data);
}

void UserManager::registerUser(std::string username, std::string password)
{
	JSON::Dict credentials = {
		{ net::MSG::USERNAME, JSON::String(username) },
		{ net::MSG::PASSWORD, JSON::String(password) }
	};
	say (net::MSG::REGISTER, credentials);
}

void UserManager::doesUserExist(std::string username)
{
	say(net::MSG::USER_EXISTS, JSON::String(username));
}

void UserManager::treatMessage(std::string const & type, JSON::Value const * data)
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