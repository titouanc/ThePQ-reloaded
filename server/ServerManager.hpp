#ifndef __SERVER_MANAGER_HPP
#define __SERVER_MANAGER_HPP

#include <map>
#include <deque>
#include <sharedqueue.hpp>
#include <network/ConnectionManager.hpp>
#include <model/User.hpp>
#include "MatchManager.hpp"

class ServerManager
{
protected:
	SharedQueue<net::Message> &_inbox, &_outbox;
	std::map<int, User*> &_users;
	net::ConnectionManager &_connectionManager;
    std::deque<MatchManager*> &_matches;
    
    User *getUserByName(std::string username);
    void sendTeamInfos(const JSON::Dict &data, int peer_id);

public:
	ServerManager(
		SharedQueue<net::Message> &inbox,
		SharedQueue<net::Message> &outbox,
		std::map<int, User*> &users,
		net::ConnectionManager &connectionManager,
		std::deque<MatchManager*> &matches) : 
		_inbox(inbox), _outbox(outbox), _users(users),
		_connectionManager(connectionManager),
		_matches(matches)
	{}

	ServerManager(const ServerManager& other) : _inbox(other._inbox), _outbox(other._outbox),
		_users(other._users), _connectionManager(other._connectionManager), 
		_matches(other._matches)
	{}
};


#endif // __SERVER_MANAGER_HPP