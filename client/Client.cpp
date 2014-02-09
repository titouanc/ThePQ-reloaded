#include "Client.hpp"

Client::Client()
{
	_socket.connect("127.0.0.1", 6666);
}

Client::~Client()
{
	
}

Client::run()
{
	
}
