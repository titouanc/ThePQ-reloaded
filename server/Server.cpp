#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>
#include <Constants.hpp>
#include "Server.hpp"
#include <sys/stat.h>
#include <model/MemoryAccess.hpp>

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

static void* runTimeLoop(void* args)
{
	Server* server = (Server*) args;
	server->timeLoop();
	pthread_exit(NULL);
}

Server::Server(NetConfig const & config) : 
	_inbox(), _outbox(), _users(),
	_connectionManager(_inbox, _outbox, config.ip.c_str(), config.port, config.maxClients),
	market(new PlayerMarket(this)),_matches()
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
	delete market;
	_matches.clear();
}

void Server::run()
{
	srand(time(NULL));	// rand() is used throughout some modules
	pthread_create(&_timeThread, NULL, runTimeLoop, this);
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
	squad1.squad_owner = _users[client_idA]->getUsername();
	delete json;

	json = JSON::load("fixtures/squad2.json");
	if (! ISDICT(json))
		return;
	Squad squad2(DICT(json));
	squad2.client_id = client_idB;
	squad2.squad_owner = _users[client_idB]->getUsername();
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
				/* Disconnect user */
				map<int, User*>::iterator it = _users.find(message.peer_id);
				if (it != _users.end()){
					delete it->second;
					_users.erase(it);
				}
			} else if (ISDICT(received.get("data"))){
				if (messageType == MSG::LOGIN){
					User *loggedIn = logUserIn(DICT(received.get("data")), message.peer_id);
					/* If someone successfully logged in; send his offline 
					   messages and reset queue */
					if (loggedIn){
						JSON::List const & offlineMsg = loggedIn->getOfflineMsg();
						for (size_t i=0; i<offlineMsg.len(); i++){
							_outbox.push(Message(message.peer_id, offlineMsg[i]->clone()));
						}
						loggedIn->clearOfflineMsg();
						loggedIn->loadTeam();
						sendTeamInfos(loggedIn->getTeam(), message.peer_id);
					}
				}
				else if(messageType == MSG::USER_CHOOSE_TEAMNAME)
					checkTeamName(DICT(received.get("data")), message.peer_id);
				else if (messageType == MSG::REGISTER) 
					registerUser(DICT(received.get("data")), message.peer_id);
				else if (messageType == MSG::ADD_PLAYER_ON_MARKET_QUERY){
					addPlayerOnMarket(DICT(received.get("data")), message.peer_id);
				}
				else if (messageType == MSG::BID_ON_PLAYER_QUERY){
					placeBidOnPlayer(DICT(received.get("data")), message.peer_id);
				}
				else if(messageType == MSG::PLAYERS_LIST) {
					sendPlayersList(message.peer_id);
				}
				else if(messageType == MSG::FRIENDLY_GAME_INVITATION_RESPONSE) {
						sendInvitationResponseToPlayer(DICT(received.get("data")), message.peer_id);
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
				} else if(messageType == MSG::FRIENDLY_GAME_USERNAME) {
						sendInvitationToPlayer(data, message.peer_id);
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

User *Server::logUserIn(const JSON::Dict &credentials, int peer_id)
{
	User *res = NULL;
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response;
		response.set("type", MSG::STATUS);
		if (getUserByName(username)){
			response.set("data", MSG::ALREADY_LOGGED_IN);
		} else {
			User *user = User::load(username);
			if (user != NULL){
				if (user->getPassword() == password){
					// correct password
					// load the user's team info into Team (installations, players, funds, etc.)
					user->loadTeam();
					// mapping user to its peer_id to keep a list of connected users.
					_users.insert(std::pair<int, User*>(peer_id, user));
					if(user->getTeam().getName() == gameconfig::UNNAMED_TEAM)
						response.set("data",MSG::USER_CHOOSE_TEAMNAME);
					else
					{
						response.set("data", MSG::USER_LOGIN);
					}
					res = user;
				} else {
					// wrong password
					delete user;
					response.set("data", MSG::PASSWORD_ERROR);
				}
			} else {
				// user not found
				response.set("data", MSG::USER_NOT_FOUND);
			}
		}
		_outbox.push(Message(peer_id, response.clone()));
	}
	return res;
}

void Server::sendTeamInfos(const JSON::Dict &data, int peer_id)
{
	JSON::Dict response;
	response.set("type", net::MSG::TEAM_INFOS);
	response.set("data", data);
	_outbox.push(Message(peer_id, response.clone()));
}

void Server::checkTeamName(const JSON::Dict &data, int peer_id){
	std::vector<std::string> teamNames;
	std::string teamname = STR(data.get(net::MSG::TEAMNAME)).value();
	bool exists = false;
	try{
		MemoryAccess::load(teamNames,memory::ALL_TEAM_NAMES);
	}
	catch(JSON::IOError e){}
	for(size_t i =0;i<teamNames.size();++i){
		if(teamNames[i]==teamname)
			exists = true;
	}
	JSON::Dict response;
	response.set("type", MSG::STATUS);
	if(!exists){
		teamNames.push_back(teamname);
		MemoryAccess::save(teamNames,memory::ALL_TEAM_NAMES);
		Team & team = _users[peer_id]->getTeam();
		team.setName(teamname);
		team.saveInfos();
		response.set("data",MSG::TEAMNAME_REGISTERED);
	}
	else{
		response.set("data",MSG::TEAMNAME_ERROR);
	}
	Message status(peer_id, response.clone());
	_outbox.push(status);

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
	JSON::List jsonInst;
	std::vector<Installation*> & insts = _users[peer_id]->getTeam().getInstallations();
	for(size_t i = 0;i<insts.size();++i){
		jsonInst.append(JSON::Dict(*insts[i]));
	}
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATIONS_LIST);
	msg.set("data", jsonInst);
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::upgradeInstallation(int peer_id, size_t i)
{
	bool res = _users[peer_id]->getTeam().upgradeInstallation(i);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_UPGRADE);
	msg.set("data", JSON::Bool(res));
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::downgradeInstallation(int peer_id, size_t i)
{
	bool res = _users[peer_id]->getTeam().downgradeInstallation(i);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	msg.set("data", JSON::Bool(res));
	_outbox.push(Message(peer_id, msg.clone()));
}

void Server::sendConnectedUsersList(int peer_id)
{
	JSON::List list;
	std::string userFrom = _users.find(peer_id)->second->getUsername();
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++)
	{
		if (it->second->getUsername() != userFrom)
		{
			list.append(it->second->getUsername());
		}
	}

	JSON::Dict usersList;
	usersList.set("type", MSG::CONNECTED_USERS_LIST);
	usersList.set("data", list);

	_outbox.push(Message(peer_id, usersList.clone()));
}

void Server::sendInvitationToPlayer(string const& userTo, int peer_id){
	map<int, User*>::iterator findUser = _users.find(peer_id);
	std::string userFrom = findUser->second->getUsername();
	cout << "from: " << userFrom;
	cout << "to: " << userTo;
	map<int, User*>::iterator it = _users.begin();
	bool found = false;
	if (userFrom != userTo)
	{
		while (it!=_users.end()){
			if (it->second->getUsername() == userTo){
				found = true;
				JSON::Dict toSend;
				toSend.set("type", MSG::FRIENDLY_GAME_INVITATION);
				toSend.set("data", userFrom);
				Message status(it->first, toSend.clone());
				_outbox.push(status);
			}
			it++;
		}
	}
	if (userFrom == userTo || !found)
	{
		JSON::Dict toSend, data;
		data.set("username", userTo);
		data.set("answer", MSG::USER_NOT_FOUND);
		toSend.set("type", MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
		toSend.set("data", data);
		Message status(peer_id, toSend.clone());
		_outbox.push(status);
	}
}

void Server::sendInvitationResponseToPlayer(const JSON::Dict &response, int peer_id){
	string username;
	if (ISSTR(response.get("username"))) 
		username = STR(response.get("username")).value();
	string answer;
	if (ISSTR(response.get("answer")))
		answer = STR(response.get("answer")).value();
	map<int, User*>::iterator findSender = _users.find(peer_id);
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++){
		if (it->second->getUsername() == username){
			JSON::Dict toSend;
			toSend.set("type", MSG::FRIENDLY_GAME_INVITATION_RESPONSE);
			JSON::Dict data;
			data.set("username", findSender->second->getUsername());
			data.set("answer", answer);
			toSend.set("data", data);
			Message status(it->first, toSend.clone());
			_outbox.push(status);
			if (answer == MSG::FRIENDLY_GAME_INVITATION_ACCEPT){
				startMatch(peer_id, it->first);
			}
		}
	}
}

void Server::addPlayerOnMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, market->addPlayer(sale).clone());
	_outbox.push(status);
}

void Server::sendPlayersOnMarketList(int peer_id){
	Message status(peer_id, market->allSales().clone());
	_outbox.push(status);
}

void Server::placeBidOnPlayer(const JSON::Dict &bid, int peer_id){
	Message status(peer_id, market->bid(bid).clone());
	_outbox.push(status);
}

void Server::sendPlayersList(int peer_id){
	std::vector<Player> & players = _users[peer_id]->getTeam().getPlayers();
	JSON::List jsonPlayers;
	for(size_t i = 0; i<players.size();++i){
		jsonPlayers.append(JSON::Dict(players[i]));
	}
	JSON::Dict toSend;
	toSend.set("type",net::MSG::PLAYERS_LIST);
	toSend.set("data", jsonPlayers);
	Message status(peer_id, toSend.clone());
	_outbox.push(status);
}

int Server::getPeerID(std::string const &username){
	for (map<int, User*>::iterator it=_users.begin(); it!=_users.end(); it++){
		if (it->second->getUsername() == username){
			return it->first;
		}
	}
	return -1;
}

void Server::sendMarketMessage(std::string const &username, const JSON::Dict &message){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::MARKET_MESSAGE);
	toSend.set("data",message);

	int to_peer = getPeerID(username);
	if (to_peer >= 0){
		/* User currently connected to server */
		Message status(to_peer, toSend.clone());
		_outbox.push(status); 
	} else {
		User *user = User::load(username);
		if (user != NULL){
			user->sendOfflineMsg(toSend);
			delete user;
		}
	}
}

User *Server::getUserByName(std::string username)
{
	std::map<int, User*>::const_iterator iter;
	for (iter=_users.begin(); iter!=_users.end(); iter++)
		if (username == iter->second->getUsername())
			return iter->second;
	return NULL;
}

void Server::timeLoop()
{
	time_t timeStart = time(NULL);
	if (timeStart != -1)
	{
		time_t timePrev = timeStart, timeNow;
		while (_connectionManager.isRunning() == true)
		{
			do
			{
				sleep(5);
				timeNow = time(NULL);
			}
			while (timeNow - timePrev < 10);
			cout << "It is  : " << ctime(&timeNow);
			timePrev = timeNow;
			timeUpdateStadium();
			timeUpdateChampionship();
		}
	}
}

void Server::timeUpdateStadium()
{
	vector<User> users;
	MemoryAccess::load(users);
	for (size_t i = 0; i < users.size(); ++i)
	{
		users[i].loadTeam();
		cout << "[" << users[i].getTeam().getName() << "] before : " << users[i].getTeam().getFunds();
		users[i].getTeam().timeUpdate();
		cout << " $ | after : " << users[i].getTeam().getFunds() << " $" << endl;
	}
}

void Server::timeUpdateChampionship()
{
	for (size_t i = 0; i < _championships.size(); ++i)
	{
		Schedule* next = _championships[i]->nextMatch();
		while (next != NULL)
		{
			next->isHappening = true;
			int clientIDA, clientIDB;
			map<int, User*>::iterator it = _users.begin();
			while (it != _users.end())
			{
				if (it->second->getUsername() == next->user1)
				{
					clientIDA = it->first;
				}
				else if (it->second->getUsername() == next->user2)
				{
					clientIDB = it->first;
				}
				it++;
			}
			startMatch(clientIDA, clientIDB);
			next = _championships[i]->nextMatch();
		}
	}
}
