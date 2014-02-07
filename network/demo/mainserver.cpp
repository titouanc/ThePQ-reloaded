#include <iostream>
#include <cstdlib>
#include "TcpSocket.hpp"
#include "Listener.hpp"
using namespace std;
using namespace net;

int main (int argc, char **argv)
{
	// Store the status of each function. OK or ERROR
	Socket::Status status;
	
	// Listen for new connections on port 6666
	Listener list;
	status = list.listen(6666);
	
	bool running = true;
	while (running)
	{
		// Accept a connection, create new socket et put it in TcpSocket
		TcpSocket socket;
		status = list.accept(socket);
	
		char* data = new char[6];
		size_t received;
		
		// Receive data of size 6. received is the actual size received
		status = socket.recv(data, 6, received);
		
		// Send data of size 6
		status = socket.send("server", 6);
		
		delete data;
	}
	return 0;
}
