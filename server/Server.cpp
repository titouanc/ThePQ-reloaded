#include "Server.hpp"
#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>
#include <Constants.hpp>

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
	_connectionManager(_inbox, _outbox, config.ip.c_str(), config.port, config.maxClients),
	_matches()
{
	_connectionManager.start();
	cout << "Launched server on " << _connectionManager.ip() << ":" << _connectionManager.port() << endl;
}

Server::~Server()
{
	std::deque<MatchManager*>::iterator it;
	for (it=_matches.begin(); it!=_matches.end(); it++){
		(*it)->stop();
		delete *it;
	}
	_matches.clear();
}

void Server::run()
{
	srand(time(NULL));	// rand() is used throughout some modules
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

void Server::collectFinishedMatches(void)
{
	std::deque<MatchManager*>::iterator it, next;
	for (it=_matches.begin(); it<_matches.end();){
		next = it;
		next++;
		if(! (*it)->isRunning()){
			delete *it;
			_matches.erase(it);
		}
		it = next;
	}
}

void Server::startMatch(int client_idA, int client_idB)
{
	collectFinishedMatches();
	if (_users.find(client_idA) == _users.end() || 
		_users.find(client_idB) == _users.end()){
		return;
	}
	JSON::Value *json = JSON::load("fixtures/squad1.json");
	if (! ISDICT(json))
		return;
	Squad squad1(DICT(json));
	squad1.client_id = client_idA;
	delete json;

	json = JSON::load("fixtures/squad2.json");
	if (! ISDICT(json))
		return;
	Squad squad2(DICT(json));
	squad2.client_id = client_idB;
	delete json;

	while (_outbox.available()); /* Clear outbox (do not lose msgs) */
	MatchManager *match = new MatchManager(_connectionManager, squad1, squad2);
	match->start();
	_matches.push_back(match);
}

void Server::treatMessage(const Message &message)
{
	if (ISDICT(message.data)){
		JSON::Dict const &received = DICT(message.data);
		if (ISSTR(received.get("type"))) {
			string messageType = STR(received.get("type")).value();
			if (messageType == MSG::DISCONNECT){
				map<int, User*>::iterator it = _users.find(message.peer_id);
				if (it != _users.end()){
					delete it->second;
					_users.erase(it);
				}
			} else if (ISDICT(received.get("data"))){
				if (messageType == MSG::LOGIN)
					logUserIn(DICT(received.get("data")), message.peer_id);
				else if (messageType == MSG::REGISTER) 
					registerUser(DICT(received.get("data")), message.peer_id);
				else if (messageType == MSG::DELETE_PLAYER_OF_MARKET_QUERY){
					deletePlayerOfMarket(DICT(received.get("data")), message.peer_id);
				}
				else if (messageType == MSG::ADD_PLAYER_ON_MARKET_QUERY){
					addPlayerOnMarket(DICT(received.get("data")), message.peer_id);
				}
				else if (messageType == MSG::BID_ON_PLAYER_QUERY){
					placeBidOnPlayer(DICT(received.get("data")), message.peer_id);
				}
				else if(messageType == MSG::PLAYERS_LIST) {//modif
					sendPlayersList(DICT(received.get("data")), message.peer_id);
				}
			} else if (ISSTR(received.get("data"))){
				string const & data = STR(received.get("data"));

				if (messageType == MSG::USER_EXISTS) {
					checkIfUserExists(data, message.peer_id);
				} else if (messageType == MSG::INSTALLATIONS_LIST){
						sendInstallationsList(message.peer_id);
				} else if(messageType == MSG::CONNECTED_USERS_LIST){
						sendConnectedUsersList(message.peer_id);
				} else if(messageType == MSG::PLAYERS_ON_MARKET_LIST) {
						sendPlayersOnMarketList(message.peer_id);
				} 
			} else if (ISINT(received.get("data"))) {
				int data = INT(received.get("data"));
				if (messageType == MSG::INSTALLATION_UPGRADE) {
					upgradeInstallation(message.peer_id, data);
				} else if (messageType == MSG::INSTALLATION_DOWNGRADE) {
					downgradeInstallation(message.peer_id, data);
				}
			} else if (messageType == "42"){
				if (_users.size() >= 2){
					std::map<int, User*>::iterator i;
					int other = -1;
					for (i=_users.begin(); i!=_users.end(); i++){
						if (i->first != message.peer_id){
							other = i->first;
							break;
						}
					}
					if (other > 0)
						startMatch(message.peer_id, other);
				}
				else {
					cout << "NOT ENOUGH PLAYERS" << endl;
					std::map<int, User*>::iterator it;
					for (it=_users.begin(); it!=_users.end(); it++)
						cout << it->first << ": " << it->second->getUsername() << endl;
				}
			}
		}
	}
}

void Server::registerUser(const JSON::Dict &credentials, int peer_id)
{
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response = JSON::Dict();
		
		response.set("type", MSG::STATUS);

		User* newUser = User::load(username);
		if (newUser != NULL){
			response.set("data", MSG::USER_EXISTS);
		} else { // User doesnt exist
			newUser = new User(username, password);
			newUser->createUser(); 
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

void Server::logUserIn(const JSON::Dict &credentials, int peer_id)
{
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response;
		response.set("type", MSG::STATUS);

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

void Server::checkIfUserExists(string username, int peer_id)
{
	User *user = User::load(username);
	JSON::Dict response;
	response.set("type", MSG::STATUS);

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

void Server::sendInstallationsList(int peer_id)
{
	string listPath = _users[peer_id]->getUserDirectoryPath() + "installations.json";
	JSON::Value * installationsList = JSON::load(listPath);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATIONS_LIST);
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

void Server::sendConnectedUsersList(int peer_id)
{
	JSON::List list;
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++)
		list.append(it->second->getUsername());

	JSON::Dict usersList;
	usersList.set("type", MSG::CONNECTED_USERS_LIST);
	usersList.set("data", list);

	_outbox.push(Message(peer_id, usersList.clone()));
}

void Server::deletePlayerOfMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, market.deletePlayer(sale).clone());
	_outbox.push(status);
}

void Server::addPlayerOnMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, market.addPlayer(sale).clone());
	_outbox.push(status);
}

void Server::sendPlayersOnMarketList(int peer_id){
	Message status(peer_id, market.allSales().clone());
	_outbox.push(status);
}
void Server::placeBidOnPlayer(const JSON::Dict &bid, int peer_id){
	Message status(peer_id, market.bid(bid).clone());
	_outbox.push(status);
}

void Server::sendPlayersList(const JSON::Dict &data, int peer_id){//modif
	std::string username = STR(data.get(net::MSG::USERNAME));
	std::string path = "data/users/" + username + "/"+"players.json";
	JSON::Dict playersList;
	JSON::List players = LIST(JSON::load(path.c_str()));
	playersList.set("type",net::MSG::PLAYERS_LIST);
	playersList.set("data", players);
	Message status(peer_id, playersList.clone());
	_outbox.push(status);
	
}
