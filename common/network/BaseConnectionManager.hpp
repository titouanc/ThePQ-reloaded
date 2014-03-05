#ifndef DEFINE_BASECONNECTIONMANAGER_HEADER
#define DEFINE_BASECONNECTIONMANAGER_HEADER 

#include <set>
#include <netinet/in.h>
#include <pthread.h>
#include <sharedqueue.hpp>
#include <json/json.hpp>

namespace net {
	struct Message {
		int peer_id;
		JSON::Value *data;
		Message(int p, JSON::Value *d) : peer_id(p), data(d) {}
	};

	/*
     * Maintain a set of file descriptors; read from and write to them.
     * start(): spawn 1 read and 1 write thread (to incoming & from outgoing)
	 */
	
	class BaseConnectionManager {
		private:
			/* Communication threads */
			pthread_t   _in_thread, _out_thread;
			/* isRunning() mutex , fd set manipulation mutex */
			pthread_mutex_t _mutex, _fdset_mutex;
			/* fd_set condtionnal wait */
			pthread_cond_t _fdset_cond;
			/* Is IO thread running ? */
			bool _running;
			/* Currently connected clients */
			std::set<int> _clients;
		protected:
			/* Incoming message queue */
			SharedQueue<Message> & _incoming, & _outgoing;
			/* Log exchanged messages */
			bool _logger;
			/* Call to select, and read from all known selected clients */
			int  _doSelect(int fdmax, fd_set *readable);
			/* Write obj to fd */
			bool _doWrite(int fd, const JSON::Value *obj);
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
				SharedQueue<Message> & outgoing_queue,
				bool logger=true
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

			/* Number of connected clients */
			size_t nClients(void);

			/* Manually feed incoming box */
			void transmit(Message const & msg);

			bool hasClient(int client_id);

			/* Main in loop: feed incoming queue */
			virtual void _mainloop_in(void);
			/* Main out loop: eat outgoing queue */
			virtual void _mainloop_out(void);
	};
}

#endif
