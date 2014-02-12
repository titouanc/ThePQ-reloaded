#include "Connection.hpp"

using namespace std;

Connection::Connection()
{
	_isLogged = false;
	_socket.connect("172.23.218.99", 32123);
}

bool Connection::isLogged()
{
	return _isLogged;
}

void Connection::loginUser(string username, string passwd)
{
	JSON::Dict toSend, credentials;
	credentials.set("CO_U", username);
	credentials.set("CO_P", passwd);
	toSend.set("type", "CO_L");
	toSend.set("data", credentials);
	_socket.send(&toSend);

	cout << "Please wait..." << endl;

	JSON::Value *serverMessage = _socket.recv();	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == "CO_S"){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == "P_ER")
					throw WrongPasswordException();
				else if (STR(received.get("data")).value() == "U_N_F")
					throw UserNotFoundException();
			}
		}
	}
	// User is logged in at this point.
}

bool Connection::doesUserExist(string username){
	bool res = false;
	JSON::Dict toSend;
	toSend.set("type", "CO_U");
	toSend.set("data", username);
	_socket.send(&toSend);

	cout << "Please wait..." << endl;

	JSON::Value *serverMessage = _socket.recv(); // receiving server response

	if(ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == "CO_S"){
			if (received.hasKey("data") && ISSTR(received.get("data")) 
				&& STR(received.get("data")).value() == "U_REG"){
				res = true;
			}
		}
	}
	
	return res;
}

void Connection::registerUser(string username, string passwd)
{
	JSON::Dict toSend, received, credentials;
	credentials.set("CO_U", username);
	credentials.set("CO_P", passwd);
	toSend.set("type", "CO_R");
	toSend.set("data", credentials);
	_socket.send(&toSend);

	// TODO receive response from server
	// TODO handle exception if response is different than U_REG
}
