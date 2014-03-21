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
#include <utility>

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
	_matches(), 
	_serverMgr(_inbox, _outbox, _users, _connectionManager, _matches),
	_userMgr(_serverMgr), _stadiumMgr(_serverMgr), _teamMgr(_serverMgr), _market(_serverMgr),
	_gameMgr(_serverMgr),
	_adminManager(_connectionManager,this),
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
			MatchResult result;
			std::pair<std::string,unsigned int> winner = (*it)->getWinner();
			std::pair<std::string,unsigned int> loser = (*it)->getLoser();
			int random = rand() % 2;
			(random == 0) ? result.setHost(winner.first) : result.setHost(loser.first); 
			result.setTeams(winner.first,loser.first);
			result.setScore(winner.second,loser.second);
			if( (*it)->isChampMatch()){
				Championship* champ = getChampionshipByUsername(result.getWinner());
				if (champ != NULL){
					result.compute(true,champ->getTurn(),champ->getTotalTurns(),champ->getCashPrize());
					champ->endMatch(result);
				}
			}
			else{
				result.compute();
			}
			endOfMatchTeamInfosUpdate(result.getWinner(),result.getWinnerMoneyGain(),result.getWinnerFameGain(),result.getWinnerAPGain());
			endOfMatchTeamInfosUpdate(result.getLoser(),result.getLoserMoneyGain(),result.getLoserFameGain(),result.getLoserAPGain());
			//Send end of match rapports
			JSON::Dict toWinner;
			JSON::Dict dataW;
			toWinner.set("type",net::MSG::END_OF_MATCH_RAPPORT);
			dataW.set(net::MSG::USERNAME,result.getLoser());
			dataW.set(net::MSG::WON_MATCH,JSON::Bool(true));
			dataW.set(net::MSG::CHAMPIONSHIP_MATCH,JSON::Bool((*it)->isChampMatch()));
			dataW.set(net::MSG::FAME_WON,result.getWinnerFameGain());
			dataW.set(net::MSG::AP_WON,result.getWinnerAPGain());
			dataW.set(net::MSG::MONEY_GAIN,result.getWinnerMoneyGain());
			toWinner.set("data",dataW);
			_serverMgr.sendNotification(result.getWinner(),toWinner);

			JSON::Dict toLoser;
			JSON::Dict dataL;
			toLoser.set("type",net::MSG::END_OF_MATCH_RAPPORT);
			dataL.set(net::MSG::USERNAME,result.getWinner());
			dataL.set(net::MSG::WON_MATCH,JSON::Bool(false));
			dataL.set(net::MSG::CHAMPIONSHIP_MATCH,JSON::Bool((*it)->isChampMatch()));
			dataL.set(net::MSG::FAME_WON,result.getLoserFameGain());
			dataL.set(net::MSG::AP_WON,result.getLoserAPGain());
			dataL.set(net::MSG::MONEY_GAIN,result.getLoserMoneyGain());
			toLoser.set("data",dataL);
			_serverMgr.sendNotification(result.getLoser(),toLoser);

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
			User *loggedIn = _userMgr.logUserIn(dict, peer_id);
			/* If someone successfully logged in; send his offline 
			   messages and reset queue */
			if (loggedIn){
				JSON::List const & offlineMsg = loggedIn->getOfflineMsg();
				for (size_t i=0; i<offlineMsg.len(); i++){
					_outbox.push(Message(peer_id, offlineMsg[i]->clone()));
				}
				loggedIn->clearOfflineMsg();
				loggedIn->loadTeam();
				_serverMgr.sendTeamInfos(loggedIn->getTeam(), peer_id);
			}
		}
		else if(messageType == MSG::USER_CHOOSE_TEAMNAME)
			_userMgr.checkTeamName(dict, peer_id);

		else if (messageType == MSG::REGISTER) 
			_userMgr.registerUser(dict, peer_id);

		else if (messageType == MSG::ADD_PLAYER_ON_MARKET_QUERY)
			_market.addPlayerOnMarket(dict, peer_id);
		
		else if (messageType == MSG::BID_ON_PLAYER_QUERY)
			_market.placeBidOnPlayer(dict, peer_id);
		
		else if(messageType == MSG::PLAYERS_LIST)
			_serverMgr.sendPlayersList(peer_id);
		
		else if(messageType == MSG::FRIENDLY_GAME_INVITATION_RESPONSE)
			_gameMgr.sendInvitationResponseToPlayer(dict, peer_id);	

		else if(messageType == MSG::PUT_PLAYER_ON_SQUAD_POSITION)
			_teamMgr.putPlayerOnSquadPosition(dict, peer_id);

		else if(messageType == MSG::SWAP_PLAYERS_SQUAD_POSITION)
			_teamMgr.swapPlayersOfSquad(dict, peer_id);
	} 

	else if (ISSTR(payload)){
		string const & data = STR(payload);

		if (messageType == MSG::INSTALLATIONS_LIST){
				_stadiumMgr.sendInstallationsList(peer_id);
		} else if(messageType == MSG::CONNECTED_USERS_LIST){
				_gameMgr.sendConnectedUsersList(peer_id);
		} else if(messageType == MSG::PLAYERS_ON_MARKET_LIST) {
				_market.	sendPlayersOnMarketList(peer_id);
		} else if(messageType == MSG::FRIENDLY_GAME_USERNAME) {
				_gameMgr.sendInvitationToPlayer(data, peer_id);
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
			_stadiumMgr.upgradeInstallation(peer_id, data);
		} else if (messageType == MSG::INSTALLATION_DOWNGRADE) {
			_stadiumMgr.downgradeInstallation(peer_id, data);
		}
	}
}

void Server::endOfMatchTeamInfosUpdate(std::string username, int money, int fame, int ap)
{
	int peer_id = _serverMgr.getPeerID(username);
	if(peer_id >= 0){
		Team & team = _users[peer_id]->getTeam();
		(money>=0) ? team.getPayed(money) : team.buy(abs(money));		//Should never lose money
		(fame>=0) ? team.earnFame(fame) : team.loseFame(abs(fame));	
		(ap>=0) ? team.earnAcPoints(ap) : team.loseAcPoints(abs(ap));	//Should never lose AP
		team.saveInfos();
	}
	else{
		Team team(username);
		team.loadInfos();
		(money>=0) ? team.getPayed(money) : team.buy(abs(money));
		(fame>=0) ? team.earnFame(fame) : team.loseFame(abs(fame));
		(ap>=0) ? team.earnAcPoints(ap) : team.loseAcPoints(abs(ap));
		team.saveInfos();
	}
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
			cout << endl;
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
		if( (peer_id = _serverMgr.getPeerID(users[i].getUsername())) >= 0 ){
			Team & team = _users[peer_id]->getTeam();
			before = team.getFunds();
			team.timeUpdate();
			team.save();
			after = team.getFunds();
			name = team.getName();
			JSON::Dict toSend;
			toSend.set("funds",team.getFunds());
			_serverMgr.sendTeamInfos(toSend,peer_id);
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
			Message toO(_serverMgr.getPeerID(opponent), toOtherUser.clone());
			_outbox.push(toO); 
			endOfPending(*pending);
			_gameMgr.startMatch(peer_id, _serverMgr.getPeerID(opponent),true);
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
			_serverMgr.sendNotification(opponent,toOtherUser);
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

void Server::timeUpdateChampionship()
{
	for (size_t i = 0; i < _championships.size(); ++i)
	{
		std::cout << *(_championships[i]) << std::endl;
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
			_serverMgr.sendNotification(_championships[i]->getWinner(),toSend);
			MemoryAccess::removeObject(*(_championships[i]));
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
		Message status(_serverMgr.getPeerID(winner),toWinner.clone());
		_outbox.push(status);
		_serverMgr.sendNotification(loser,toLoser);
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
		_serverMgr.sendNotification(winner,toWinner);
		_serverMgr.sendNotification(loser,toLoser);
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
	_serverMgr.sendNotification(username,toSend);
}

void Server::notifyStartingChampionship(Championship & champ){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_STATUS_CHANGE);
	toSend.set("data",net::MSG::CHAMPIONSHIP_STARTED);
	std::vector<std::string> users = champ.getUsers();
	for(size_t i = 0; i < users.size(); ++i){
		_serverMgr.sendNotification(users[i],toSend);
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
		if(!(*it)->isStarted() && !(*it)->isFull())
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








