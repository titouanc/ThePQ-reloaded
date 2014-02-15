#include "Connection.hpp"

using namespace std;

Connection::Connection(std::string host, int port)
{
	_socket.connect(host, port);
}

void Connection::loginUser(string username, string passwd)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::LOGIN_QUERY);
	toSend.set("data", credentials);
	_socket.send(&toSend);

	JSON::Value *serverMessage = _socket.recv();	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::CONNECTION_STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == net::MSG::PASSWORD_ERROR)
					throw WrongPasswordException();
				else if (STR(received.get("data")).value() == net::MSG::USER_NOT_FOUND)
					throw UserNotFoundException();
			}
		}
	}
	// User is logged in at this point.
}

void Connection::doesUserExist(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS_QUERY);
	toSend.set("data", username);
	_socket.send(&toSend);

	JSON::Value *serverMessage = _socket.recv(); // receiving server response

	if(ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::CONNECTION_STATUS){
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
	toSend.set("type", net::MSG::REGISTER_QUERY);
	toSend.set("data", credentials);
	_socket.send(&toSend);

	JSON::Value *serverMessage = _socket.recv();	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::CONNECTION_STATUS){
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
	query.set("type", net::MSG::DATA_QUERY);
	query.set("data", net::MSG::INSTALLATIONS_LIST);
	_socket.send(&query);

	JSON::Value *serverResponse = _socket.recv();
	if (ISLIST(serverResponse))
	{
		toFill = LIST(serverResponse);
	}
	vector<Installation> vec;
	for (size_t i = 0; i < toFill.len(); ++i)
	{
		vec.push_back(DICT(toFill[i]));
	}
	delete serverResponse;
	return vec;
}

void Connection::getConnectedUsersList(vector<string> &users){
	JSON::Dict query;
	query.set("type", net::MSG::DATA_QUERY);
	query.set("data", net::MSG::CONNECTED_USERS_LIST);
	_socket.send(&query);

	JSON::Value *serverResponse = _socket.recv();
	if (ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & connectedUsers = LIST(received.get("data"));
			for (size_t i = 0; i<connectedUsers.len(); ++i)
				users.push_back(STR(connectedUsers[i]));
		}
	}
}
