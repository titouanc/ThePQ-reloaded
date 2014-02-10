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
	JSON::Dict data;
	data.set("username", username);
	data.set("passwd", passwd);
	net::Message *msg = new net::Message(net::Message::Request::LOGIN, 
											net::Message::Method::REQUEST, 
											&data);
	cout << "Client sending: " << msg->dumps() << endl;
	_socket.send(msg);
	delete msg;
	//~ _socket.recv((JSON::Value**)&msg);
	//~ cout << "Client receiving: " << msg->dumps() << endl;
}

void Connection::registerUser(string username, string passwd)
{
	
}
