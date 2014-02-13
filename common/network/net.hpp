#ifndef __NET_HPP
#define __NET_HPP

/*
 * net.hpp : header for a socket library
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
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include "json.h"

namespace net
{
	class ConnectionFailedException : public std::runtime_error {
	public:
		ConnectionFailedException():runtime_error(std::string("connect() failed: ")+strerror(errno))
		{}
	};
	
	class SendFailedException : public std::runtime_error {
	public:
		SendFailedException():runtime_error(std::string("send() failed: ")+strerror(errno))
		{}
	};
	
	class RecvFailedException : public std::runtime_error {
	public:
		RecvFailedException():runtime_error(std::string("recv() failed: ")+strerror(errno))
		{}
	};
	
	namespace MSG
	{
		namespace TYPE
		{
			#define LOGIN_QUERY "Q_L"
			#define REGISTER_QUERY "Q_R"
			#define USER_EXISTS_QUERY "Q_U"
			#define USER_STATUS "Q_S"
		}
		namespace DATA
		{
			#define USERNAME "D_U"
			#define PASSWORD "D_P"
			#define USER_LOGIN "U_L"
			#define PASSWORD_ERROR "U_P"
			#define USER_NOT_FOUND "U_NF"
			#define USER_EXISTS "U_E"
			#define USER_REGISTERED "U_R"
		}
	}
	
	class TcpSocket {
	public:
		static const size_t MSG_SIZE = 4096;
	
		virtual ~ TcpSocket();
		TcpSocket();

		void connect(const std::string ipAddr, int portNo);
		void send(const char *data, size_t len);
		void recv(char *data, size_t len, size_t & received);
		
		void send(const JSON::Value *json);
		JSON::Value* recv();
		
	protected:
		bool create();
		void close();
		bool isOpen() {
			return _sockfd >= 0;
		};

		int _sockfd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _cliAddr;
	};
}

#endif // __NET_HPP
