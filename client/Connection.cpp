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
	JSON::Dict data, msg;
	data.set("username", username);
	data.set("password", passwd);
	msg.set("__type__", "LOGIN");
	msg.set("__data__", data);
	_socket.send(&msg);
	// TODO loginUser: recv response from server
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
