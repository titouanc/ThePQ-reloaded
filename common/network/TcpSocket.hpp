#ifndef __TCP_SOCKET_HPP
#define __TCP_SOCKET_HPP

/*
 * TcpSocket.hpp : header for a socket library
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

namespace net
{	
	class TcpSocket {
	public:
		static const size_t MSG_SIZE = 4096;
	
		virtual ~ TcpSocket();
		TcpSocket(std::string host, int portno);
	
		void send(JSON::Value const& json);
		JSON::Value * pop();
			
		bool available();
		
		void start();
		void stop();
		JSON::Value* waitForMsg(std::string typeToWait);
		JSON::Value* hasMessageTypeInNotifications(std::string messageType);
		void updateNotifications();
		std::queue<JSON::Value*> notifications;
		
	protected:
		SharedQueue<JSON::Value*> _messages;

		void loop();
		bool _isRunning;
		bool _isWaitingForMessage;
		int _sockfd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _cliAddr;
	};
	
	static void* runThread(void* arg)
	{
		TcpSocket* socket = (TcpSocket*)arg;
		socket->start();
		pthread_exit(NULL);
	}
}

#endif // __TCP_SOCKET_HPP
