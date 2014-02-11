#include "Server.hpp"

using namespace std;

Server::Server(){
	_connectionManager = ConnectionManager(_inbox, _outbox, "0.0.0.0", 32123);
	_connectionManager.start();
	cout << "Launched server on " << manager.ip() << ":" << manager.port() << endl;
	run();
}

void Server::run(){
	while (_connectionManager.isRunning()){
		Message const & msg = inbox.pop();
		cout << "Got message from client #" << msg.peer_id << endl;
		cout << "\t" << *(msg.data) << endl;
		treatMessage(msg);
	}
}

void Server::treatMessage(Message &message){
	cout << "Got message from client #" << message.peer_id << endl;
	cout << "\t" << *(message.data) << endl;
}