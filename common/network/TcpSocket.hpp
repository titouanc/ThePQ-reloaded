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
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <json/json.hpp>
#include "Exception.hpp"

namespace net
{
	
	namespace MSG
	{
		// TYPE ----------------------------------------------
		static const std::string LOGIN_QUERY = "Q_L";
		static const std::string REGISTER_QUERY = "Q_R";
		static const std::string USER_EXISTS_QUERY = "Q_U";
		static const std::string CONNECTION_STATUS = "Q_S";
		static const std::string DATA_QUERY = "Q_D";
		static const std::string INSTALLATION_UPGRADE = "AQ_I_U";
		static const std::string INSTALLATION_DOWNGRADE = "AQ_I_D";
		
		// DATA ----------------------------------------------
		// connection + register
		static const std::string USERNAME = "D_U";
		static const std::string PASSWORD = "D_P";
		static const std::string USER_LOGIN = "U_L";
		static const std::string PASSWORD_ERROR = "U_P";
		static const std::string USER_NOT_FOUND = "U_NF";
		static const std::string USER_EXISTS = "U_E";
		static const std::string USER_REGISTERED = "U_R";
		// data queries
		static const std::string INSTALLATIONS_LIST = "DQ_I_L";
		static const std::string CONNECTED_USERS_LIST = "DQ_CU_L";

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

#endif // __TCP_SOCKET_HPP
