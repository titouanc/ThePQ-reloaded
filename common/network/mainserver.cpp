#include <iostream>
#include <cstdlib>
#include "net.hpp"
#include "json.h"
#include "Message.hpp"
using namespace std;
using namespace net;

int main (int argc, char **argv)
{
	// Store the status of each function. OK or ERROR
	Socket::Status status;
	
	// Listen for new connections on port 6666
	Listener list;
	status = list.listen(6666);
	cout << "listen: " << status << endl;
	if (status == Socket::Status::ERROR)
	{
		cout << "Listener.listen: ERROR" << endl;
		exit(1);
	}
	
	bool running = true;
	while (running)
	{
		// Accept a connection, create new socket et put it in TcpSocket
		TcpSocket socket;
		status = list.accept(socket);
		cout << "accept: " << status << endl;	
	
		// Create JSON value and assign with received data
		JSON::Value *data;
		Message *msg;
		status = socket.recv((JSON::Value**) &msg);
		cout << "Server: recv: " << msg->dumps() << endl;
		// You must delete because recv use JSON::parse which allocate memory
		
		delete msg;
		
		// Create JSON value and send it
		data = JSON::parse("42");
		msg = new Message(Message::Request::LOGIN, Message::Method::RESPONSE, data);
		cout << "Server: send: " << msg->dumps() << endl;
		status = socket.send(msg);
		
		delete data;
		delete msg;
		
		//~ char* data = new char[6];
		//~ size_t received;
		//~ 
		//~ // Receive data of size 6. received is the actual size received
		//~ status = socket.recv(data, 6, received);
		//~ cout << "recv: " << status << endl;
		//~ 
		//~ // Send data of size 6
		//~ status = socket.send("server", 6);
		//~ cout << "send: " << status << endl;
		//~ delete data;
	}
	return 0;
}