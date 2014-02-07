#ifndef __LISTENER_HPP
#define __LISTENER_HPP

#include "TcpSocket.hpp"

class Listener:Socket {
  public:
    static const int CONNECTION_NB = 20;

    Status listen(int portNo);
    Status accept(TcpSocket & sock);
};

#endif				// __LISTENER_HPP
