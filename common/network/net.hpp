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
#include "json.h"

namespace net
{
	class Socket {
	  public:
		virtual ~ Socket();

		enum Status {
		ERROR,
		OK
		};

	  protected:
		Socket();
		bool create();
		void close();
		bool isOpen() {
			return _sockfd >= 0;
		};

		int _sockfd;
		struct sockaddr_in _servAddr;
		struct sockaddr_in _cliAddr;
	};
	
	class Listener;
	
	class TcpSocket:public Socket {
		friend Listener;
	public:
		static const size_t MSG_SIZE = 4096;
	  
		Status connect(const std::string ipAddr, int portNo);
		Status send(const char *data, size_t len);
		Status recv(char *data, size_t len, size_t & received);
		
		Status send(const JSON::Value *json);
		Status recv(JSON::Value **json);
	};
	
	class Listener:public Socket {
	  public:
		static const int CONNECTION_NB = 20;

		Status listen(int portNo);
		Status accept(TcpSocket & sock);
	};
	
	class Message : public JSON::Dict
	{
	public:		
		enum Method
		{
			REQUEST,
			RESPONSE
		};
		
		enum Request
		{
			LOGIN,
			REGISTER
		};
		
		Message(Request request, Method method, JSON::Value* data) : JSON::Dict()
		{
			set("Request", JSON::Integer(request));
			set("Method", JSON::Integer(method));
			set("Data", *data);
		}
		
		Message(JSON::Dict const & dict)
		{
			set("Request", *(dict.get("Request")));
			set("Method", *(dict.get("Method")));
			set("Data", *(dict.get("Data")));
		}
		
		~Message()
		{
			
		}	
	};
}

#endif // __NET_HPP
