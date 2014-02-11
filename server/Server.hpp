#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <iostream>
#include <string>
#include "../common/network/connectionmanager.h"

class Server{
public:
	Server();
	void run();
	void treatMessage(Message &message);
private:
	ConnectionManager _connectionManager;
	SharedQueue<Message> _inbox, _outbox;
};
#endif