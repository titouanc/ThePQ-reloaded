#include "Connection.hpp"
#include <iostream>
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
	JSON::Dict* data = new JSON::Dict();
	data->set("username", JSON::String(username));
	data->set("passwd", JSON::String(passwd));
	net::Message *msg = new net::Message(net::Message::Request::LOGIN, 
											net::Message::Method::REQUEST, 
											data);
	_socket.send(msg);
	cout << "Client sending: " << msg->dumps() << endl;
	delete msg;
	delete data;
	_socket.recv((JSON::Value**)&msg);
	cout << "Client receiving: " << msg->dumps() << endl;
}

void Connection::registerUser(string username, string passwd)
{
	
}
