#ifndef DEFINE_CONNECTIONMANAGER_HEADER
#define DEFINE_CONNECTIONMANAGER_HEADER

#include "BaseConnectionManager.hpp"

namespace net {
	/*
     * A connection manager that first bind(), then accept() in read loop.
	 */
	class ConnectionManager : public BaseConnectionManager {
		private:
			/* Bound IP structure */
			struct sockaddr_in _bind_addr;
			/* Listener socket */
			int _sockfd;
		public:
			explicit ConnectionManager(
				SharedQueue<Message> & incoming_queue,
				SharedQueue<Message> & outgoing_queue,
				const char *bind_addr="127.0.0.1", 
				unsigned short bind_port=32123,
				int max_clients=25
			);
			~ConnectionManager();

			void _mainloop_in(void);

			/* Getters */
			const char *ip(void) const;
			unsigned short port(void) const;
	};
}

#endif
