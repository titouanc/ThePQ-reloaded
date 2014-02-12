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
			&& (received.hasKey("data") && ISDICT(received.get("data")))){
			string messageType = STR(received.get("type")).value();
			if (messageType == "CO_L"){
				logUserIn(DICT(received.get("data")), message.peer_id);
			}
			else if (messageType == "CO_R") {
				registerUser(received.get("data"), message.peer_id);
			}
		}
	}
}

void Server::registerUser(const JSON::Dict &credentials, int peer_id){
	if (credentials.hasKey("CO_U") && ISSTR(credentials.get("CO_U"))
		&& credentials.hasKey("CO_P") && ISSTR(credentials.get("CO_P" ))){
		string userpath = "data/users/" + STR(credentials.get("CO_U")).value();
		User newUser(STR(credentials.get("CO_U")).value(), STR(credentials.get("CO_P")).value());
		newUser.save();
	}
}

void Server::logUserIn(const JSON::Dict &credentials, int peer_id){
	if (credentials.hasKey("CO_U") && ISSTR(credentials.get("CO_U"))
		&& credentials.hasKey("CO_P") && ISSTR(credentials.get("CO_P" ))){
		User user(STR(credentials.get("CO_U")).value(), STR(credentials.get("CO_P")).value());
	}
	JSON::Dict * statusDict = new JSON::Dict();
	statusDict->set("type", "CO_S");
	statusDict->set("data", "U_L_IN");
	Message status(peer_id, statusDict);
	_outbox.push(status);
}
