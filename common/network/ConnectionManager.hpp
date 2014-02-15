#ifndef DEFINE_BaseConnectionManager_HEADER
#define DEFINE_BaseConnectionManager_HEADER

#include <iostream>
#include <set>
#include <stdexcept>
#include <netinet/in.h>
#include <pthread.h>
#include <sharedqueue.hpp>
#include <json/json.hpp>
#include "Exception.hpp"

namespace net {
	struct Message {
		int peer_id;
		JSON::Value *data;
		Message(int p, JSON::Value *d) : peer_id(p), data(d) {}
	};

	class BaseConnectionManager {
		private:
			/* Communication threads */
			pthread_t   _in_thread, _out_thread;
			/* isRunning() mutex , fd set manipulation mutex */
			pthread_mutex_t _mutex, _fdset_mutex;
			/* Is IO thread running ? */
			bool _running;
			/* Currently connected clients */
			std::set<int> _clients;
		protected:
			/* Incoming message queue */
			SharedQueue<Message> & _incoming, &_outgoing;
			/* Call to select, and read from all known selected clients */
			int  _doSelect(int fdmax, fd_set *readable);
			/* Write obj to fd */
			bool _doWrite(int fd, JSON::Value *obj);
			/* Read a json object from fd and push it in incoming queue */
			bool _doRead(int fd);
			/* Close fd, remove client from managed clients and 
			   emit DISCONNECT message */
			void _doDisconnect(int fd);
			/* Add client to managed clients and emit CONNECT message */
			void _doConnect(int fd);
			
			/* iterator on managed clients */
			typedef std::set<int>::iterator iterator;
			iterator _iterClients(void){return _clients.begin();}
			iterator _iterEnd(void){return _clients.end();}
		public:
			explicit BaseConnectionManager(
				SharedQueue<Message> & incoming_queue,
				SharedQueue<Message> & outgoing_queue
			);
			~BaseConnectionManager();

			/* Start the main IO loops in a background thread */
			bool start(void);

			/* Stop the main IO loops */
			bool stop(void);

			/* Add fd to managed clients */
			void addClient(int fd);

			/* Remove fd from managed clients */
			bool removeClient(int fd);

			/* Are the I/O threads up ? */
			bool isRunning(void);

			/* Main in loop: feed incoming queue */
			virtual void _mainloop_in(void);
			/* Main out loop: eat outgoing queue */
			void _mainloop_out(void);
	};

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

	class SubConnectionManager : public BaseConnectionManager {
		private:
			BaseConnectionManager & _parent;
		public:
			/* ConnectionManager which takes its managed clients from a parent
			   ConnectionManager. It cannot accept connections by itself. */
			SubConnectionManager(
			    SharedQueue<Message> & incoming_queue,
			    SharedQueue<Message> & outgoing_queue,
			    BaseConnectionManager & parent
			);
			/* Remove client_id from parent (if any) and add to self. */
			bool acquireClient(int client_id);
			/* Remove client_id from self (if any) and add to parent. */
			bool releaseClient(int client_id);
			/* Remove all managed clients before destroying. */
			~SubConnectionManager();
	};
	
	class ClientConnectionManager : public BaseConnectionManager {
		private:
			struct sockaddr_in _host_addr;
			int _sockfd;
		public:
			explicit ClientConnectionManager(
				SharedQueue<Message> & incoming_queue,
				SharedQueue<Message> & outgoing_queue,
				const char *host_addr="127.0.0.1", 
				unsigned short host_port=32123
			);
			~ClientConnectionManager();

			/* Getters */
			const char *ip(void) const;
			unsigned short port(void) const;
	};
}

#endif
