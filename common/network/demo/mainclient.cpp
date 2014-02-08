#include <iostream>
#include <cstdlib>
#include "net.hpp"
using namespace std;
using namespace net;

int main (int argc, char **argv)
{
	// Store the status of each function. OK or ERROR
	Socket::Status status;
	
	// Connect to server at 127.0.0.1, port 6666
	TcpSocket socket;
	status = socket.connect("127.0.0.1", 6666);
	cout << "connect: " << status << endl;
	
	// Send data of size 6
	status = socket.send("client", 6);
	cout << "send: " << status << endl;
	
	// Receive data of size 6. received is the actual size received
	size_t received;
	char* data = new char[10];
	status = socket.recv(data, 6, received);
	cout << "recv: " << status << endl;
	
	delete data;
	return 0;
}
