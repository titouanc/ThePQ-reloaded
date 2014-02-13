#include "Connection.hpp"

using namespace std;

Connection::Connection()
{
	_socket.connect("0.0.0.0", 32123);
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
