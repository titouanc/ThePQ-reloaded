#include "Server.hpp"

using namespace std;

Server::Server() : _connectionManager(_inbox, _outbox, "0.0.0.0", 32123){
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
		if (received.hasKey("type") && ISSTR(received.get("type"))) {
			string messageType = STR(received.get("type")).value();
			if (received.hasKey("data") && ISDICT(received.get("data"))){
				// data is a DICT
				if (messageType == net::MSG::LOGIN_QUERY){
					logUserIn(DICT(received.get("data")), message.peer_id);
				}
				else if (messageType == net::MSG::REGISTER_QUERY) {
					registerUser(DICT(received.get("data")), message.peer_id);
				}
			}
			else if (received.hasKey("data") && ISSTR(received.get("data"))){
				// data is a STR
				if (messageType == net::MSG::USER_EXISTS_QUERY) {
					checkIfUserExists(STR(received.get("data")).value(), message.peer_id);
				}
			}
		}
	}
}

void Server::registerUser(const JSON::Dict &credentials, int peer_id){
	if (credentials.hasKey(net::MSG::USERNAME) && ISSTR(credentials.get(net::MSG::USERNAME))
		&& credentials.hasKey(net::MSG::PASSWORD) && ISSTR(credentials.get(net::MSG::PASSWORD))){
		User* newUser = User::load(STR(credentials.get(net::MSG::USERNAME)).value());
		if (newUser == NULL)
		{
			// User doesnt exist
			newUser = new User(STR(credentials.get(net::MSG::USERNAME)).value(), STR(credentials.get(net::MSG::PASSWORD)).value());
			newUser->save();
			JSON::Dict * statusDict = new JSON::Dict();
			statusDict->set("type", net::MSG::CONNECTION_STATUS);
			statusDict->set("data", net::MSG::USER_REGISTERED);
			Message status(peer_id, statusDict);
			_outbox.push(status);
		}
		else
		{
			// User exists
			JSON::Dict * statusDict = new JSON::Dict();
			statusDict->set("type", net::MSG::CONNECTION_STATUS);
			statusDict->set("data", net::MSG::USER_EXISTS);
			Message status(peer_id, statusDict);
			_outbox.push(status);			
		}
		delete newUser;
	}
}

void Server::logUserIn(const JSON::Dict &credentials, int peer_id){
	if (credentials.hasKey(net::MSG::USERNAME) && ISSTR(credentials.get(net::MSG::USERNAME))
		&& credentials.hasKey(net::MSG::PASSWORD) && ISSTR(credentials.get(net::MSG::PASSWORD))){
		User *user = User::load(STR(credentials.get(net::MSG::USERNAME)).value());
		if (user != NULL){
			if (user->getPassword() == STR(credentials.get(net::MSG::PASSWORD)).value()){
				// correct password
			JSON::Dict * statusDict = new JSON::Dict();
			statusDict->set("type", net::MSG::CONNECTION_STATUS);
			statusDict->set("data", net::MSG::USER_LOGIN);
			Message status(peer_id, statusDict);
			_outbox.push(status);
			}
			else {
				// wrong password
				JSON::Dict * statusDict = new JSON::Dict();
				statusDict->set("type", net::MSG::CONNECTION_STATUS);
				statusDict->set("data", net::MSG::PASSWORD_ERROR);
				Message status(peer_id, statusDict);
				_outbox.push(status);
			}
		}
		else{
			// user not found
			JSON::Dict * statusDict = new JSON::Dict();
			statusDict->set("type", net::MSG::CONNECTION_STATUS);
			statusDict->set("data", net::MSG::USER_NOT_FOUND);
			Message status(peer_id, statusDict);
			_outbox.push(status);
		}	
	}
}

void Server::checkIfUserExists(string username, int peer_id){
	User *user = User::load(username);
	if (user != NULL){
		// user found
		JSON::Dict * statusDict = new JSON::Dict();
		statusDict->set("type", net::MSG::CONNECTION_STATUS);
		statusDict->set("data", net::MSG::USER_EXISTS);
		Message status(peer_id, statusDict);
		_outbox.push(status);
	}
	else {
		// user not found
		JSON::Dict * statusDict = new JSON::Dict();
		statusDict->set("type", net::MSG::CONNECTION_STATUS);
		statusDict->set("data", net::MSG::USER_NOT_FOUND);
		Message status(peer_id, statusDict);
		_outbox.push(status);
	}
}
