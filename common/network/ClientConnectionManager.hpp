#ifndef __CLIENT_CONNECTION_MANAGER_HPP
#define __CLIENT_CONNECTION_MANAGER_HPP

/*
 * ClientConnectionManager.hpp : header for a socket library
 * 
 * Author : Antoine Carpentier
 * 
 * Date : 08/02/2014
 * 
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <json/json.hpp>
#include "Exception.hpp"
#include <sharedqueue.hpp>
#include <queue>
#include <pthread.h>

namespace net
{	
	class ClientConnectionManager {
	public:
		virtual ~ClientConnectionManager();
		ClientConnectionManager(std::string host, int portno);
	
		void send(JSON::Value const& json);
		
		void start();
		
		JSON::Value* waitForMsg(std::string typeToWait);
		JSON::Value* getNotification(std::string messageType);
		JSON::Value* popMessage();
		SharedQueue<JSON::Value*>& getNotifications();
		void updateNotifications();
		
	protected:
		SharedQueue<JSON::Value*> _notifications;
		SharedQueue<JSON::Value*> _messages;
		
		pthread_t _thread;

		void loop();
		bool _isRunning;
		bool _isWaitingForMessage;
		int _sockfd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _cliAddr;
	};
}

#endif // __CLIENT_CONNECTION_MANAGER_HPP
