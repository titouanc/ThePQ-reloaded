#include "Server.hpp"
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;
using namespace net;

std::string humanExcName(const char *name)
{
	int status;
	char *str = abi::__cxa_demangle(name, 0, 0, &status);
	std::string res(str);
	free(str);
	return res;
}

Server::Server(NetConfig const & config) : 
	_inbox(), _outbox(), _users(),
	_connectionManager(_inbox, _outbox, config.ip.c_str(), config.port, config.maxClients)
{
	_connectionManager.start();
	cout << "Launched server on " << _connectionManager.ip() << ":" << _connectionManager.port() << endl;
}

void Server::run(){
	while (_connectionManager.isRunning() || _inbox.available()){
		Message const & msg = _inbox.pop();
		try {treatMessage(msg);}
		catch (std::runtime_error & err){
			cerr << "\033[31mError " << humanExcName(typeid(err).name()) 
				 << " in handler of " << *(msg.data) << endl
			     << "\t" << err.what() << "\033[0m" << endl;
		} catch (...){
			cerr << "\033[31mUnknow error in handler of " << *(msg.data) 
				 << "\033[0m" << endl;
		}
		delete msg.data;
	}
}

void Server::treatMessage(const Message &message){
	if (ISDICT(message.data)){
		JSON::Dict const &received = DICT(message.data);
		if (ISSTR(received.get("type"))) {
			string messageType = STR(received.get("type")).value();
			if (messageType == "DISCONNECT"){
				map<int, User*>::iterator it = _users.find(message.peer_id);
				if (it != _users.end()){
					delete it->second;
					_users.erase(it);
				}
			} else if (ISDICT(received.get("data"))){
				if (messageType == MSG::LOGIN_QUERY)
					logUserIn(DICT(received.get("data")), message.peer_id);
				else if (messageType == MSG::REGISTER_QUERY) 
					registerUser(DICT(received.get("data")), message.peer_id);
			} else if (ISSTR(received.get("data"))){
				string const & data = STR(received.get("data"));

				if (messageType == MSG::USER_EXISTS_QUERY) {
					checkIfUserExists(data, message.peer_id);
				} else if (messageType == MSG::DATA_QUERY){
					if (data == MSG::INSTALLATIONS_LIST)
						sendInstallationsList(message.peer_id);
					else if(data == MSG::CONNECTED_USERS_LIST)
						sendConnectedUsersList(message.peer_id);
				}
			} else if (ISINT(received.get("data"))) {
				int data = INT(received.get("data"));
				if (messageType == MSG::INSTALLATION_UPGRADE) {
					upgradeInstallation(message.peer_id, data);
				} else if (messageType == MSG::INSTALLATION_DOWNGRADE) {
					downgradeInstallation(message.peer_id, data);
				}
			}
		}
	}
}

void Server::registerUser(const JSON::Dict &credentials, int peer_id){
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response = JSON::Dict();
		response.set("type", MSG::CONNECTION_STATUS);

		User* newUser = User::load(username);
		if (newUser != NULL){
			response.set("data", MSG::USER_EXISTS);
		} else { // User doesnt exist
			newUser = new User(username, password);
			newUser->save();
			response.set("data", MSG::USER_REGISTERED);
		}

		/* Clone dict before sending, because
		     + it should be dynamically allocated (ConnectionManager deletes it)
		     + cloning at the end is less memory efficient, but do not leak
		       memory if a trap/exc occurs after response allocation.
		 */
		Message status(peer_id, response.clone());
		_outbox.push(status);
		delete newUser;
	}
}

void Server::logUserIn(const JSON::Dict &credentials, int peer_id){
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response;
		response.set("type", MSG::CONNECTION_STATUS);

		User *user = User::load(username);
		if (user != NULL){
			if (user->getPassword() == password){
				// correct password
				// mapping user to its peer_id to keep a list of connected users.
				_users.insert(std::pair<int, User*>(peer_id, user));
				response.set("data", MSG::USER_LOGIN);
			} else {
				// wrong password
				response.set("data", MSG::PASSWORD_ERROR);
			}
		} else {
			// user not found
			response.set("data", MSG::USER_NOT_FOUND);
		}

		Message status(peer_id, response.clone());
		_outbox.push(status);
	}
}

void Server::checkIfUserExists(string username, int peer_id){
	User *user = User::load(username);
	JSON::Dict response;
	response.set("type", MSG::CONNECTION_STATUS);

	if (user != NULL)
		// user found
		response.set("data", MSG::USER_EXISTS);
	else 
		// user not found
		response.set("data", MSG::USER_NOT_FOUND);

	Message status(peer_id, response.clone());
	_outbox.push(status);

	delete user;
}

void Server::sendInstallationsList(int peer_id){
	string listPath = _users[peer_id]->getUserDirectoryPath() + "installations.json";
	JSON::Value * installationsList = JSON::load(listPath);
	JSON::Dict msg;
	msg.set("type", net::MSG::DATA_SEND);
	msg.set("data", *installationsList);
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::upgradeInstallation(int peer_id, size_t i)
{
	_users[peer_id]->getInstallations()[i].upgrade();
	_users[peer_id]->saveInstallations();
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_UPGRADE);
	msg.set("data", JSON::Bool(true));
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::downgradeInstallation(int peer_id, size_t i)
{
	_users[peer_id]->getInstallations()[i].downgrade();	
	_users[peer_id]->saveInstallations();
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	msg.set("data", JSON::Bool(true));
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::sendConnectedUsersList(int peer_id){
	JSON::List list;
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++)
		list.append(it->second->getUsername());

	JSON::Dict usersList;
	usersList.set("type", MSG::CONNECTED_USERS_LIST);
	usersList.set("data", list);

	_outbox.push(Message(peer_id, usersList.clone()));
}
