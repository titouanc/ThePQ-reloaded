#include "Server.hpp"
#include "../common/network/connectionmanager.h"

using namespace std;

int main(int argc, const char **argv)
{
	SharedQueue<Message> inbox, outbox;

	try {
		ConnectionManager manager(inbox, outbox, "0.0.0.0", 32123);
		manager.start();
		cout << "Launched server on " << manager.ip() << ":" << manager.port() << endl;
		while (manager.isRunning()){
			Message const & msg = inbox.pop();
			cout << "Got message from client #" << msg.peer_id << endl;
			cout << "\t" << *(msg.data) << endl;
			outbox.push(msg);
		}
	} catch (ConnectionError & err){
		cout << "ERREUR: " << err.what() << endl;
	}
	return 0;
}