#include "UserManager.hpp"

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
	if (type == net::MSG::LOGIN)
	{
		std::string const & response = STR(data).value();
		if (response == net::MSG::USER_LOGIN)
			onLoginUser();
		else if (response == net::MSG::USER_CHOOSE_TEAMNAME)
			onAskTeamName();
		else
			onLoginError(response);
	}
	else if (type == net::MSG::REGISTER)
	{
		onRegisterUser(STR(data));
	}
	else if (type == net::MSG::TEAMNAME)
	{
		onTeamName(STR(data));
	}
}