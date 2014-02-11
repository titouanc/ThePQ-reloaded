#include "Connection.hpp"

using namespace std;

Connection::Connection()
{
	_isLogged = false;
	_socket.connect("127.0.0.1", 6666);
}

bool Connection::isLogged()
{
	return _isLogged;
}

void Connection::loginUser(string username, string passwd)
{
	if (doesUserExist(username)){
		toSend.set("type", "CO_P");
		toSend.set("data", encrypt(passwd, HASH_KEY));
		_socket.send(&toSend);

		// TODO receive response from server

		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")) == "CO_S"){
			if (received.hasKey("data") && ISSTR(received.get("data")) 
				&& STR(received.get("data")) != "P_OK"){
				throw WrongPasswordException();
			}
		}
		// User is logged in at this point.
	}
	else {
		throw UserNotFoundException();
	}
}

bool Connection::doesUserExist(string username){
	bool res = false;
	JSON::Dict toSend, received;
	toSend.set("type", "CO_U");
	toSend.set("data", username);
	_socket.send(&toSend);

	// TODO receive server response

	if (received.hasKey("type") && ISSTR(received.get("type")) 
		&& STR(received.get("type")) == "CO_S"){
		if (received.hasKey("data") && ISSTR(received.get("data")) 
			&& STR(received.get("data")) == "U_REG"){
			res = true;
		}
	}
	return res;
}

void Connection::registerUser(string username, string passwd)
{
	JSON::Dict data, msg;
	data.set("username", username);
	data.set("password", passwd);
	msg.set("__type__", "REGISTER");
	msg.set("__data__", data);
	_socket.send(&msg);
	// TODO registerUser: recv response from server	
}
