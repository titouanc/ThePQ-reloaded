#include "Connection.hpp"

using namespace std;

Connection::Connection(std::string host, int port) : _inbox(), _outbox(),
_connectionManager(_inbox, _outbox, host.c_str(), port)
{
	_connectionManager.start();
}

void Connection::loginUser(string username, string passwd)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data;	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
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
		}
	}
	delete serverMessage;
	// User is logged in at this point.
}

void Connection::doesUserExist(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS);
	toSend.set("data", username);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data; // receiving server response

	if(ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data")) 
				&& STR(received.get("data")).value() == net::MSG::USER_EXISTS){
				throw UserAlreadyExistsException();
			}
		}
	}
}

void Connection::registerUser(string username, string passwd)
{
	JSON::Dict toSend, received, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data;	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == net::MSG::USER_EXISTS)
					throw UserAlreadyExistsException();
			}
		}
	}
	// User is registered
}

vector<Installation> Connection::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	net::Message msg(0, query.clone());
	_outbox.push(msg);

	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict response = DICT(serverResponse);
		if (ISLIST(response.get("data")))
		{
			toFill = LIST(response.get("data"));
		}
	}
	vector<Installation> vec;
	for (size_t i = 0; i < toFill.len(); ++i)
	{
		vec.push_back(DICT(toFill[i]));
	}
	delete serverResponse;
	return vec;
}

bool Connection::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	
	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict const & received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_UPGRADE)
		{
			ret = received.get("data");
		}
	}
	return ret;
}

bool Connection::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	
	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict const &received = DICT(serverResponse);
		
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_DOWNGRADE)
		{
			ret = received.get("data");
		}
	}
	delete serverResponse;
	return ret;
}

vector<std::string> Connection::getConnectedUsersList(){
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	net::Message msg(0, query.clone());
	_outbox.push(msg);

	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & connectedUsers = LIST(received.get("data"));
			for (size_t i = 0; i<connectedUsers.len(); ++i)
				res.push_back(STR(connectedUsers[i]));
		}
	}
	return res;
}
