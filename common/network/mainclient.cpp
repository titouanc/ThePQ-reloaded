#include <iostream>
#include <cstdlib>
#include "net.hpp"
#include "json.h"
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
	
	// Parse JSON value then send it
	JSON::Value *data = JSON::parse("\"La reponse a la grande question sur l'univers, la vie et sur tout ce qui est\"");
	socket.send(data);
	cout << "Client: send: " << data->dumps() << endl;
	
	delete data;
	
	// Receive data by passing the address of a pointer to JSON::Value
	socket.recv(&data);
	cout << "Client: recv: " << data->dumps() << endl;
	
	delete data;
	
	//~ // Send data of size 6
	//~ status = socket.send("client", 6);
	//~ cout << "send: " << status << endl;
	//~ 
	//~ // Receive data of size 6. received is the actual size received
	//~ size_t received;
	//~ char* data = new char[10];
	//~ status = socket.recv(data, 6, received);
	//~ cout << "recv: " << status << endl;
	//~ 
	//~ delete data;
	return 0;
}
