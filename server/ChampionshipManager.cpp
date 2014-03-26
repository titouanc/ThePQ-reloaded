#include "ChampionshipManager.hpp"

void ChampionshipManager::loadChampionships(){
	MemoryAccess::load(_championships);
	for(size_t i = 0; i < _championships.size(); ++i){
		if(_championships[i]->isStarted()){
			_championships[i]->clearSchedules();
			_championships[i]->start();
		}
	}
}

Championship* ChampionshipManager::getChampionshipByUsername(std::string username){
	std::deque<Championship*>::iterator it;
	for(it=_championships.begin();it<_championships.end();it++){
		if( (*it)->isUserIn(username) == true){
			return *it;
		}
	}
	return NULL;
}

Championship* ChampionshipManager::getChampionshipByName(std::string champName){
	std::deque<Championship*>::iterator it;
	for(it=_championships.begin();it<_championships.end();it++){
		if( (*it)->getName() == champName){
			return *it;
		}
	}
	return NULL;
}

void ChampionshipManager::responsePendingChampMatch(std::string response, int peer_id){
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
			startMatch(peer_id, getPeerID(opponent),true);
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

void ChampionshipManager::resolveUnplayedChampMatch(Schedule & pending){
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

void ChampionshipManager::endOfPending(Schedule & sche){
	for(size_t i = 0; i < _pendingChampMatches.size(); ++i){
		if(_pendingChampMatches[i].user1 == sche.user1){
			_pendingChampMatches.erase(_pendingChampMatches.begin()+i);
		}
	}
}

Schedule* ChampionshipManager::getPendingMatchByUsername(std::string username){
	std::deque<Schedule>::iterator it;
	for(it=_pendingChampMatches.begin();it<_pendingChampMatches.end();++it){
		if((*it).user1 == username || (*it).user2 == username)
			return &(*it);
	}
	return NULL;
}

void ChampionshipManager::notifyPendingChampMatch(std::string username){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_MATCH_PENDING);
	toSend.set("data","");
	sendNotification(username,toSend);
}

void ChampionshipManager::notifyStartingChampionship(Championship & champ){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::CHAMPIONSHIP_STATUS_CHANGE);
	toSend.set("data",net::MSG::CHAMPIONSHIP_STARTED);
	std::vector<std::string> users = champ.getUsers();
	for(size_t i = 0; i < users.size(); ++i){
		sendNotification(users[i],toSend);
	}
}

void ChampionshipManager::sendChampionshipsList(int peer_id){
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

void ChampionshipManager::sendJoinedChampionship(int peer_id){
	JSON::Dict toSend;
	toSend.set("type",net::MSG::JOINED_CHAMPIONSHIP);
	pthread_mutex_lock(&_champsMutex);
	Championship* champ = getChampionshipByUsername(_users[peer_id]->getUsername());
	pthread_mutex_unlock(&_champsMutex);
	(champ == NULL) ? toSend.set("data",net::MSG::CHAMPIONSHIP_NOT_FOUND) : toSend.set("data",JSON::Dict(*champ));
	Message status(peer_id,toSend.clone());
	_outbox.push(status);
}

void ChampionshipManager::leaveChampionship(int peer_id){
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

void ChampionshipManager::joinChampionship(std::string champName, int peer_id){
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