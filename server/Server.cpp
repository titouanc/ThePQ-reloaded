#include "Server.hpp"

using namespace std;

Server::Server() : _connectionManager(ConnectionManager(_inbox, _outbox, "0.0.0.0", 32123)){
	_connectionManager.start();
	cout << "Launched server on " << _connectionManager.ip() << ":" << _connectionManager.port() << endl;
	run();
}

void Server::run(){
	while (_connectionManager.isRunning()){
		Message const & msg = _inbox.pop();
		treatMessage(msg);
	}
}

void Server::treatMessage(const Message &message){
	cout << "Got message from client #" << message.peer_id << endl;
	cout << "\t" << *(message.data) << endl;
	if (ISDICT(message.data)){
		JSON::Dict const &received = DICT(message.data);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == "CO_L"){
			if (received.hasKey("data") && ISDICT(received.get("data"))) {
				// TODO check user credentials
				JSON::Dict statusDict;
				statusDict.set("type", "CO_S");
				statusDict.set("data", "U_L_IN");
				Message status(message.peer_id, statusDict.clone());
				_outbox.push(status);
			}
		}
	}
}