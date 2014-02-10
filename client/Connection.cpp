#include "Connection.hpp"
#include "Exception.hpp"

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
	throw UserNotFoundException();
}

void Connection::registerUser(string username, string passwd)
{
	
}
