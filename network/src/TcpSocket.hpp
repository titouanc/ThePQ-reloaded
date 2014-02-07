#ifndef __TCP_SOCKET_HPP
#define __TCP_SOCKET_HPP

#include "Socket.hpp"
#include <string>

namespace net
{
	class Listener;

	class TcpSocket:Socket {
		friend Listener;
	  public:
		Status connect(const std::string ipAddr, int portNo);
		Status send(const void *data, size_t len);
		Status recv(void *data, size_t len, size_t & received);
	};
}

#endif				// __TCP_SOCKET_HPP
