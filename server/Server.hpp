#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <iostream>
#include <string>
#include "../common/network/connectionmanager.h"

using namespace std;

class Server{
public:
	Server();
	void run();
	void treatMessage(const Message &message);

	void logUserIn(const JSON::Dict &credentials, int peer_id);
private:
	ConnectionManager _connectionManager;
	SharedQueue<Message> _inbox, _outbox;
};
#endif