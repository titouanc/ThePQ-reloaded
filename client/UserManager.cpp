#include "UserManager.hpp"

UserManager::UserManager(net::ClientConnectionManager& connection, UserData& user) : 
	_connection(connection), _user(user)
{
}

void UserManager::loginUser(std::string username, std::string password)
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
		std::string const & payload = STR(received.get("data"));
		if (payload == net::MSG::PASSWORD_ERROR)
		{
			delete serverMessage;
			throw WrongPasswordException();
		}
		else if (payload == net::MSG::USER_NOT_FOUND)
		{
			delete serverMessage;
			throw UserNotFoundException();
		}
		else if (payload == net::MSG::ALREADY_LOGGED_IN)
		{
			delete serverMessage;
			throw AlreadyLoggedInException();
		}
		else if (payload == net::MSG::USER_CHOOSE_TEAMNAME)
		{
			_user.login(username);
			delete serverMessage;
			throw NoTeamNameException();
		}

	}
	delete serverMessage;
	_user.login(username);
}

void UserManager::logoutUser()
{
	JSON::Dict toSend;
	toSend.set("type", net::MSG::DISCONNECT);
	toSend.set("data", "");
	_connection.send(toSend);
	_user.logout();
}

void UserManager::chooseTeamName(std::string username, std::string teamname){
	JSON::Dict toSend, data;
	data.set(net::MSG::TEAMNAME,teamname);
	data.set(net::MSG::USERNAME,username);
	toSend.set("type",net::MSG::USER_CHOOSE_TEAMNAME);
	toSend.set("data",data);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS);
	JSON::Dict const & received = DICT(serverMessage);
	if (ISSTR(received.get("data"))){
		if(STR(received.get("data")).value()==net::MSG::TEAMNAME_ERROR){
			delete serverMessage;
			throw TeamNameNotAvailableException();
		}
	}
	delete serverMessage;
}

void UserManager::registerUser(std::string username, std::string password)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, password);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	_connection.send(toSend);

	JSON::Value *serverMessage = _connection.waitForMsg(net::MSG::STATUS);	// receiving server response
	JSON::Dict const & received = DICT(serverMessage);
	if (ISSTR(received.get("data"))) {
		if (STR(received.get("data")).value() == net::MSG::USER_EXISTS){
			delete serverMessage;
			throw UserAlreadyExistsException();
		}
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
