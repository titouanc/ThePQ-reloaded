#include "UserManager.hpp"

UserManager::UserManager(ClientManager const & parent) : ClientManager(parent)
{
}

void UserManager::loginUser(std::string username, std::string password)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	connection().send(toSend);
}

void UserManager::logoutUser()
{
	JSON::Dict toSend;
	toSend.set("type", net::MSG::DISCONNECT);
	toSend.set("data", "");
	connection().send(toSend);
	user().logout();
}

void UserManager::chooseTeamName(std::string username, std::string teamname)
{
	JSON::Dict toSend, data;
	data.set(net::MSG::TEAMNAME,teamname);
	data.set(net::MSG::USERNAME,username);
	toSend.set("type",net::MSG::USER_CHOOSE_TEAMNAME);
	toSend.set("data",data);
	connection().send(toSend);
}

void UserManager::registerUser(std::string username, std::string password)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	connection().send(toSend);
}

void UserManager::doesUserExist(std::string username)
{
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS);
	toSend.set("data", username);
	connection().send(toSend);
}

void UserManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::LOGIN)
	{
		onLoginUser(STR(data));
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