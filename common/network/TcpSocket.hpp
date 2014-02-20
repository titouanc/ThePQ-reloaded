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

namespace net
{	
	class TcpSocket {
	public:
		static const size_t MSG_SIZE = 4096;
	
		virtual ~ TcpSocket();
		TcpSocket(std::string host, int portno);
	
		void send(JSON::Value const& json);
		
		void loop();
		
		void start();
		void stop();
		
	protected:
		SharedQueue<JSON::Value*> _messages;

		bool _isRunning;
		int _sockfd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _cliAddr;
	};
}

#endif // __TCP_SOCKET_HPP
