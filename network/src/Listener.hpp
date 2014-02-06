#ifndef __LISTENER_HPP
#define __LISTENER_HPP

#include "TcpSocket.hpp"

class Listener : Socket
{
public:
	Status listen(int portNo);
	Status accept(TcpSocket &sock);
	
	static const int CONNECTION_NB = 20;
};

#endif // __LISTENER_HPP
