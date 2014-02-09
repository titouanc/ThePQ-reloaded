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
