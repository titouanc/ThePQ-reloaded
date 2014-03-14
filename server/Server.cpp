#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>
#include <Constants.hpp>
#include "Server.hpp"
#include <sys/stat.h>
#include <unistd.h>
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
	_market(new PlayerMarket(this)),_matches(),_adminManager(_connectionManager,this),
	_timeTicks(0), _champsMutex(PTHREAD_MUTEX_INITIALIZER), _timeThread()
{
	loadChampionships();
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
	delete _market;
	_matches.clear();
}

void Server::initDefaultAccounts()
{
	std::vector<User> users;
	MemoryAccess::load(users);
	if (users.size() == 0){
		JSON::Value *defaults = JSON::load("defaultAccounts.json");
		JSON::Dict const & toCreate = DICT(defaults);
		JSON::Dict::const_iterator it;
		for (it=toCreate.begin(); it!=toCreate.end(); it++){
			User newUser(it->first, STR(it->second));
			newUser.createUser();
			cout << "[" << this << "] Creating user " << it->first << endl;
		}
		delete defaults;
	}
}

void Server::run()
{
	initDefaultAccounts();
	time_t tstart = time(NULL);
	unsigned long long int tick = 0;
	srand(time(NULL));	// rand() is used throughout some modules
	pthread_create(&_timeThread, NULL, runTimeLoop, this);
	while (_connectionManager.isRunning() || _inbox.available()){
		Message const & msg = _inbox.pop();
		try {
			if (ISDICT(msg.data)){
				JSON::Dict const & content = DICT(msg.data);
				if (ISSTR(content.get("type")) && content.hasKey("data"))
					treatMessage(msg.peer_id, STR(content.get("type")), content.get("data"));
			}
		}
		catch (std::runtime_error & err){
			cerr << "\033[31mError " << humanExcName(typeid(err).name()) 
				 << " in handler of " << *(msg.data) << endl
			     << "\t" << err.what() << "\033[0m" << endl;
		} catch (...){
			cerr << "\033[31mUnknow error in handler of " << *(msg.data) 
				 << "\033[0m" << endl;
		}
		delete msg.data;
		tick++;
		if (tick%10 == 0){
			unsigned long long int tx = _connectionManager.txBytes()>>10;
			unsigned long long int rx = _connectionManager.rxBytes()>>10;
			time_t uptime = time(NULL) - tstart;
			cout << "[" << this << "] \033[1;47;30mNetwork statistics\033[0m :: "
				 << "sent: " << tx << "kB "
				 << "received: " << rx << "kB " 
				 << "(up " << uptime << " seconds)" << endl;
		}
	}
}

void Server::loadChampionships(){
	MemoryAccess::load(_championships);
	for(size_t i = 0; i < _championships.size(); ++i){
		if(_championships[i]->isStarted()){
			_championships[i]->clearSchedules();
			_championships[i]->start();
		}
	}
}

void Server::collectFinishedMatches(void)
{
	std::deque<MatchManager*>::iterator it, next;
	for (it=_matches.begin(); it<_matches.end();){
		next = it;
		next++;
		if(! (*it)->isRunning()){
			if( (*it)->isChampMatch()){
				MatchResult & result = (*it)->getResult();
				Championship* champ = getChampionshipByUsername(result.getWinner());
				if (champ != NULL){
					champ->endMatch(result);
				}
				result.compute(true);
				//result.save();
			}
			delete *it;
			_matches.erase(it);
		}
		it = next;
	}
}

Championship* Server::getChampionshipByUsername(std::string username){
	std::deque<Championship*>::iterator it;
	for(it=_championships.begin();it<_championships.end();it++){
		if( (*it)->isUserIn(username) == true){
			return *it;
		}
	}
	return NULL;
}

Championship* Server::getChampionshipByName(std::string champName){
	std::deque<Championship*>::iterator it;
	for(it=_championships.begin();it<_championships.end();it++){
		if( (*it)->getName() == champName){
			return *it;
		}
	}
	return NULL;
}

void Server::startMatch(int client_idA, int client_idB, bool champMatch)
{
	if (_users.find(client_idA) == _users.end() || 
		_users.find(client_idB) == _users.end()){
		return;
	}

	Squad & squad1 = _users[client_idA]->getTeam().getSquad();
	squad1.client_id = client_idA;

	Squad & squad2 = _users[client_idB]->getTeam().getSquad();
	squad2.client_id = client_idB;

	while (_outbox.available()); /* Clear outbox (do not lose msgs) */
	MatchManager *match = new MatchManager(_connectionManager, squad1, squad2, champMatch);
	match->start();
	_matches.push_back(match);
}

void Server::treatMessage(
	int peer_id, 
	std::string const & messageType, 
	const JSON::Value * payload
){
	if (messageType == MSG::DISCONNECT){
		/* Disconnect user */
		map<int, User*>::iterator it = _users.find(peer_id);
		if (it != _users.end()){
			delete it->second;
			_users.erase(it);
		}
	}
	else if (messageType == MSG::ADMIN_LOGIN){
		_adminManager.acquireClient(peer_id);
		_adminManager.start();
		JSON::Dict toTransmit = {
			{"type", JSON::String(messageType)},
			{"data", *payload}
		};
		_adminManager.transmit(Message(peer_id, toTransmit.clone()));
	}
	else if (ISDICT(payload)){
		JSON::Dict const & dict = DICT(payload);
		if (messageType == MSG::LOGIN){
			User *loggedIn = logUserIn(dict, peer_id);
			/* If someone successfully logged in; send his offline 
			   messages and reset queue */
			if (loggedIn){
				JSON::List const & offlineMsg = loggedIn->getOfflineMsg();
				for (size_t i=0; i<offlineMsg.len(); i++){
					_outbox.push(Message(peer_id, offlineMsg[i]->clone()));
				}
				loggedIn->clearOfflineMsg();
				loggedIn->loadTeam();
				sendTeamInfos(loggedIn->getTeam(), peer_id);
			}
		}
		else if(messageType == MSG::USER_CHOOSE_TEAMNAME)
			checkTeamName(dict, peer_id);

		else if (messageType == MSG::REGISTER) 
			registerUser(dict, peer_id);

		else if (messageType == MSG::ADD_PLAYER_ON_MARKET_QUERY)
			addPlayerOnMarket(dict, peer_id);
		
		else if (messageType == MSG::BID_ON_PLAYER_QUERY)
			placeBidOnPlayer(dict, peer_id);
		
		else if(messageType == MSG::PLAYERS_LIST)
			sendPlayersList(peer_id);
		
		else if(messageType == MSG::FRIENDLY_GAME_INVITATION_RESPONSE)
				sendInvitationResponseToPlayer(dict, peer_id);	

		else if(messageType == MSG::PUT_PLAYER_ON_SQUAD_POSITION)
			putPlayerOnSquadPosition(dict, peer_id);

		else if(messageType == MSG::SWAP_PLAYERS_SQUAD_POSITION)
			swapPlayersOfSquad(dict, peer_id);
	} 

	else if (ISSTR(payload)){
		string const & data = STR(payload);

		if (messageType == MSG::USER_EXISTS) {
			checkIfUserExists(data, peer_id);
		} else if (messageType == MSG::INSTALLATIONS_LIST){
				sendInstallationsList(peer_id);
		} else if(messageType == MSG::CONNECTED_USERS_LIST){
				sendConnectedUsersList(peer_id);
		} else if(messageType == MSG::PLAYERS_ON_MARKET_LIST) {
				sendPlayersOnMarketList(peer_id);
		} else if(messageType == MSG::FRIENDLY_GAME_USERNAME) {
				sendInvitationToPlayer(data, peer_id);
		} else if(messageType == MSG::JOINABLE_CHAMPIONSHIPS_LIST) {
				sendChampionshipsList(peer_id);
		} else if(messageType == MSG::JOIN_CHAMPIONSHIP) {
				joinChampionship(data, peer_id);
		} else if(messageType == MSG::LEAVE_CHAMPIONSHIP) {
				leaveChampionship(peer_id);
		} else if(messageType == MSG::CHAMPIONSHIP_MATCH_PENDING_RESPONSE){
				responsePendingChampMatch(data,peer_id);
		} else if(messageType == MSG::JOINED_CHAMPIONSHIP)
				sendJoinedChampionship(peer_id);
	} 

	else if (ISINT(payload)) {
		int data = INT(payload);
		if (messageType == MSG::INSTALLATION_UPGRADE) {
			upgradeInstallation(peer_id, data);
		} else if (messageType == MSG::INSTALLATION_DOWNGRADE) {
			downgradeInstallation(peer_id, data);
		}
	}
}

void Server::registerUser(const JSON::Dict &credentials, int peer_id)
{
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));
		JSON::Dict response = JSON::Dict();
		response.set("type", MSG::REGISTER);
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
		response.set("type", MSG::LOGIN);
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
	response.set("type", MSG::TEAMNAME);
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
	JSON::Dict cache;
	cache.set("funds",_users[peer_id]->getTeam().getFunds());
	sendTeamInfos(cache,peer_id);
}

void Server::downgradeInstallation(int peer_id, size_t i)
{
	bool res = _users[peer_id]->getTeam().downgradeInstallation(i);
	JSON::Dict msg;
	msg.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	msg.set("data", JSON::Bool(res));
	_outbox.push(Message(peer_id, msg.clone()));
	JSON::Dict cache;
	cache.set("funds",_users[peer_id]->getTeam().getFunds());
	sendTeamInfos(cache,peer_id);
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
				startMatch(peer_id, it->first, false);
			}
		}
	}
}

void Server::addPlayerOnMarket(const JSON::Dict &sale, int peer_id){
	Message status(peer_id, _market->addPlayer(sale).clone());
	_outbox.push(status);
}

void Server::sendPlayersOnMarketList(int peer_id){
	Message status(peer_id, _market->allSales().clone());
	_outbox.push(status);
}

void Server::placeBidOnPlayer(const JSON::Dict &bid, int peer_id){
	Message status(peer_id, _market->bid(bid).clone());
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

void Server::putPlayerOnSquadPosition(const JSON::Dict &response, int peer_id){
	int position = 0;
	int member_id = 0;
	if (ISINT(response.get(net::MSG::PLAYER_ID)))
		member_id = INT(response.get(net::MSG::PLAYER_ID));
	if (ISINT(response.get(net::MSG::SQUAD_POSITION)))
		position = INT(response.get(net::MSG::SQUAD_POSITION));
	_users[peer_id]->getTeam().getSquad().putPlayerAtPosition(member_id, position);
	sendTeamInfos(_users[peer_id]->getTeam(), peer_id);
	MemoryAccess::save(_users[peer_id]->getTeam());
}

void Server::swapPlayersOfSquad(const JSON::Dict &response, int peer_id){
	int position = 0;
	int member_id = 0;
	if (ISINT(response.get(net::MSG::PLAYER_ID)))
		member_id = INT(response.get(net::MSG::PLAYER_ID));
	if (ISINT(response.get(net::MSG::SQUAD_POSITION)))
		position = INT(response.get(net::MSG::SQUAD_POSITION));
	_users[peer_id]->getTeam().getSquad().swapPlayers(member_id, position);
	sendTeamInfos(_users[peer_id]->getTeam(), peer_id);
	MemoryAccess::save(_users[peer_id]->getTeam());
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
				sleep(gameconfig::SLEEP_TIME);
				timeNow = time(NULL);
			}
			while (timeNow - timePrev < gameconfig::TICK_TIME);
			++_timeTicks;
			cout << "It is  : " << ctime(&timeNow);
			timePrev = timeNow;
			if (_timeTicks % gameconfig::TICKS_BEFORE_MATCH == 0)
			{
				collectFinishedMatches();
			}
			if (_timeTicks % gameconfig::TICKS_BEFORE_CHAMPIONSHIP == 0)
			{
				timeUpdateChampionship();
			}
			if (_timeTicks % gameconfig::TICKS_BEFORE_STADIUM == 0)
			{
				timeUpdateStadium();
			}
			if (_timeTicks == gameconfig::TICKS_BEFORE_RESET)
			{
				_timeTicks = 0;
			}
		}
	}
}

void Server::timeUpdateStadium()
{
	vector<User> users;
	MemoryAccess::load(users);
	long int before, after;
	std::string name;
	for (size_t i = 0; i < users.size(); ++i)
	{
		int peer_id;
		if( (peer_id = getPeerID(users[i].getUsername())) >= 0 ){
			Team & team = _users[peer_id]->getTeam();
			before = team.getFunds();
			team.timeUpdate();
			team.save();
			after = team.getFunds();
			name = team.getName();
			JSON::Dict toSend;
			toSend.set("funds",team.getFunds());
			sendTeamInfos(toSend,peer_id);
		}
		else{
			Team team(users[i].getUsername());
			team.load();
			before = team.getFunds();
			team.timeUpdate();
			team.save();
			after = team.getFunds();
			name = team.getName();
		}

		cout << "[" << this << "] \033[33m" << name
			 << "\033[0m before: " << before << "$; after: " << after << "$" << endl;
	}
}

void Server::responsePendingChampMatch(std::string response, int peer_id){
	std::string sender = _users[peer_id]->getUsername();
	std::string opponent;
	JSON::Dict toSender, toOtherUser;
	toSender.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS);		//Normal message
	toOtherUser.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS);	//Notification
	Schedule * pending = getPendingMatchByUsername(sender);
	if(pending == NULL){
		toSender.set("data",net::MSG::CHAMPIONSHIP_MATCH_NOT_FOUND);
		Message status(peer_id, toSender.clone());
		_outbox.push(status);
		return;
	}
	else{
		pthread_mutex_lock(&_champsMutex);
		opponent = (pending->user1 == sender) ? pending->user2 : pending->user1;
		std::string & senderStatus  = (pending->user1 == sender) ? pending->statusUser1 : pending->statusUser2;
		std::string & opponentStatus = (pending->user1 == opponent) ? pending->statusUser1 : pending->statusUser2;
		//Update statuses in Schedule
		//User is ready to play match
		if(response == net::MSG::CHAMPIONSHIP_MATCH_READY){
			senderStatus = net::MSG::CHAMPIONSHIP_MATCH_READY;
		}
		//User withdrawed from match
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW){
			senderStatus = net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW;
		}
		//Compare statuses
		//Case 1 : both users are ready, which means opponent was waiting for sender to say 'ready'
		if(senderStatus == net::MSG::CHAMPIONSHIP_MATCH_READY && opponentStatus == net::MSG::CHAMPIONSHIP_MATCH_READY){
			toSender.set("data",net::MSG::CHAMPIONSHIP_MATCH_START);
			toOtherUser.set("data",net::MSG::CHAMPIONSHIP_MATCH_START);
			Message toS(peer_id, toSender.clone());
			_outbox.push(toS);
			Message toO(getPeerID(opponent), toOtherUser.clone());
			_outbox.push(toO); 
			endOfPending(*pending);
			startMatch(peer_id,getPeerID(opponent),true);
		}
		//Case 2 : sender withdrawed from match
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW){
			toSender.set("data",net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW);
			if(opponentStatus == net::MSG::CHAMPIONSHIP_MATCH_READY){
				toOtherUser.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS);
				toOtherUser.set("data",net::MSG::CHAMPIONSHIP_MATCH_OPPONENT_WITHDRAW);
			}
			else{
				toOtherUser.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE);
				toOtherUser.set("data",net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW);
			}
			Message toS(peer_id, toSender.clone());
			_outbox.push(toS);
			sendNotification(opponent,toOtherUser);
			//Resolve end of match
			endOfPending(*pending);
			MatchResult res;
			res.setTeams(opponent, sender);
			Championship* champ = getChampionshipByUsername(sender);
			if(champ != NULL){
				champ->endMatch(res);
			}
		}
		//Case 3 : sender is ready, but opponent is not
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_READY){
			toSender.set("data",net::MSG::CHAMPIONSHIP_MATCH_WAIT);
			Message toS(peer_id, toSender.clone());
			_outbox.push(toS);
		}
		pthread_mutex_unlock(&_champsMutex);
	}
}

void Server::sendNotification(std::string username, const JSON::Dict & toSend){
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

void Server::timeUpdateChampionship()
{
	for (size_t i = 0; i < _championships.size(); ++i)
	{
		if(_championships[i]->isStarted() && !(_championships[i]->areUsersNotified())){
			notifyStartingChampionship(*(_championships[i]));
			_championships[i]->usersNotified();
		}
		Schedule* next = _championships[i]->nextMatch();
		while (next != NULL)
		{
			next->isHappening = true;
			_pendingChampMatches.push_back(*next);
			notifyPendingChampMatch(next->user1);
			notifyPendingChampMatch(next->user2);
			next = _championships[i]->nextMatch();
		}
		if(_championships[i]->isEnded()){
			JSON::Dict toSend;
			toSend.set("type",net::MSG::CHAMPIONSHIP_STATUS_CHANGE);
			toSend.set("data",net::MSG::CHAMPIONSHIP_WON);
			sendNotification(_championships[i]->getWinner(),toSend);
			pthread_mutex_lock(&_champsMutex);
			delete _championships[i]; 
			_championships.erase(_championships.begin()+i);
			--i;
			pthread_mutex_unlock(&_champsMutex);
		}
	}
	//Check in waiting matches if offset is over
	time_t now = time(NULL);
	pthread_mutex_lock(&_champsMutex);
	for (size_t i = 0; i < _pendingChampMatches.size(); ++i)
	{
		if(abs(difftime(now, _pendingChampMatches[i].date)) > gameconfig::MAX_CHAMP_MATCH_OFFSET){
			//Time range over, resolve match
			resolveUnplayedChampMatch(_pendingChampMatches[i]);
		}
	}
	pthread_mutex_unlock(&_champsMutex);
}

void Server::resolveUnplayedChampMatch(Schedule & pending){
	MatchResult res;
	JSON::Dict toWinner, toLoser;
	std::string winner, loser;
	//One user at least didn't answer to the pending match notification
	if(	pending.statusUser1 == net::MSG::CHAMPIONSHIP_MATCH_READY || 
		pending.statusUser2 == net::MSG::CHAMPIONSHIP_MATCH_READY){
		if (pending.statusUser1 == net::MSG::CHAMPIONSHIP_MATCH_READY){
			winner = pending.user1;
			loser = pending.user2;
		}
		else if (pending.statusUser2 == net::MSG::CHAMPIONSHIP_MATCH_READY){
			winner = pending.user2;
			loser = pending.user1;
		}
		toWinner.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS);
		toWinner.set("data",net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_WON);
		toLoser.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE);
		toLoser.set("type",net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_LOST);
		Message status(getPeerID(winner),toWinner.clone());
		_outbox.push(status);
		sendNotification(loser,toLoser);
	}
	//Else none user responded to notification : random the winner...
	else{
		int randWinner = rand() % 2 + 1;
		winner = (randWinner == 1) ? pending.user1 : pending.user2;
		loser = (winner == pending.user1) ? pending.user2 : pending.user1;
		toWinner.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE);
		toWinner.set("data",net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_WON);
		toLoser.set("type",net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE);
		toLoser.set("data",net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_LOST);
		sendNotification(winner,toWinner);
		sendNotification(loser,toLoser);
	}
	res.setTeams(winner,loser);
	res.compute(true);
	//res.save();
	Championship* champ = getChampionshipByUsername(winner);
	if (champ != NULL) 
		champ->endMatch(res);
	endOfPending(pending);
}

void Server::endOfPending(Schedule & sche){
	for(size_t i = 0; i < _pendingChampMatches.size(); ++i){
		if(_pendingChampMatches[i].user1 == sche.user1){
			_pendingChampMatches.erase(_pendingChampMatches.begin()+i);
		}
	}
}

Schedule* Server::getPendingMatchByUsername(std::string username){
	std::deque<Schedule>::iterator it;
	for(it=_pendingChampMatches.begin();it<_pendingChampMatches.end();++it){
		if((*it).user1 == username || (*it).user2 == username)
			return &(*it);
	}
	return NULL;
}

void Server::notifyPendingChampMatch(std::string username){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_MATCH_PENDING);
	toSend.set("data","");
	sendNotification(username,toSend);
}

void Server::notifyStartingChampionship(Championship & champ){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_STATUS_CHANGE);
	toSend.set("data",net::MSG::CHAMPIONSHIP_STARTED);
	std::vector<std::string> users = champ.getUsers();
	for(size_t i = 0; i < users.size(); ++i){
		sendNotification(users[i],toSend);
	}
}

void Server::addChampionship(const Championship& champ){
	pthread_mutex_lock(&_champsMutex);
	Championship* newChamp = new Championship(champ);
	_championships.push_back(newChamp);
	MemoryAccess::save(*newChamp);
	pthread_mutex_unlock(&_champsMutex);
}

void Server::sendChampionshipsList(int peer_id){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::JOINABLE_CHAMPIONSHIPS_LIST);
	JSON::List champs;
	std::deque<Championship*>::iterator it;
	pthread_mutex_lock(&_champsMutex);
	for(it = _championships.begin();it < _championships.end();++it){
		if(!(*it)->isStarted())
			champs.append(JSON::Dict(*(*it)));
	}
	pthread_mutex_unlock(&_champsMutex);
	toSend.set("data",champs);
	Message status(peer_id, toSend.clone());
	_outbox.push(status);
}

void Server::sendJoinedChampionship(int peer_id){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::JOINED_CHAMPIONSHIP);
	pthread_mutex_lock(&_champsMutex);
	Championship* champ = getChampionshipByUsername(_users[peer_id]->getUsername());
	pthread_mutex_unlock(&_champsMutex);
	(champ == NULL) ? toSend.set("data",net::MSG::CHAMPIONSHIP_NOT_FOUND) : toSend.set("data",JSON::Dict(*champ));
	Message status(peer_id,toSend.clone());
	_outbox.push(status);
}

void Server::leaveChampionship(int peer_id){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::LEAVE_CHAMPIONSHIP);
	std::string username = _users[peer_id]->getUsername();
	pthread_mutex_lock(&_champsMutex);
	Championship* champ = getChampionshipByUsername(username);
	if (champ == NULL)
		toSend.set("data",net::MSG::NOT_IN_CHAMPIONSHIP);
	else if(champ->isStarted())
		toSend.set("data",net::MSG::CHAMPIONSHIP_STARTED);
	else{
		champ->removeUser(*(_users[peer_id]));
		toSend.set("data",net::MSG::REMOVED_FROM_CHAMPIONSHIP);
	}
	pthread_mutex_unlock(&_champsMutex);
	Message status(peer_id, toSend.clone());
	_outbox.push(status);
}

void Server::joinChampionship(std::string champName, int peer_id){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::JOIN_CHAMPIONSHIP);
	pthread_mutex_lock(&_champsMutex);
	Championship* champ = getChampionshipByName(champName);
	if(champ == NULL)
		toSend.set("data",net::MSG::CHAMPIONSHIP_NOT_FOUND);
	else if(champ->isFull())
		toSend.set("data",net::MSG::CHAMPIONSHIP_FULL);
	else if(getChampionshipByUsername(_users[peer_id]->getUsername()) != NULL)
		toSend.set("data",net::MSG::ALREADY_IN_CHAMPIONSHIP);
	else if(champ->isStarted())
		toSend.set("data",net::MSG::CHAMPIONSHIP_STARTED);
	else{
		champ->addUser(*(_users[peer_id]));
		toSend.set("data",net::MSG::ADDED_TO_CHAMPIONSHIP);
	}
	pthread_mutex_unlock(&_champsMutex);
	Message status(peer_id, toSend.clone());
	_outbox.push(status);
}








