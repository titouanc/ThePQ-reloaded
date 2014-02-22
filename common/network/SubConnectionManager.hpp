#ifndef DEFINE_SUBCONNECTIONMANAGER_HEADER
#define DEFINE_SUBCONNECTIONMANAGER_HEADER

#include "BaseConnectionManager.hpp"

namespace net {
	/*
	 * A connection manager which can temporarily borrow FDs from another CM.
	 * Useful to make temporary local dedicated communication channels (eg. matches)
	 */
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
}

#endif
