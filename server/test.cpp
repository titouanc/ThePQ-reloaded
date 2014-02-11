#include "connectionmanager.h"

using namespace std;

int main(int argc, const char **argv)
{
	SharedQueue<Message> incoming, outgoing;

	try {
		ConnectionManager manager;
		cout << "Launched server on " << manager.ip() << ":" << manager.port() << endl;
		while (true){
			manager.mainloop(incoming);
			
			while (incoming.available()){
				Message const & msg = incoming.pop();
				cout << "Message from client #" << msg.peer_id << endl;
				if (ISDICT(msg.data)){
					JSON::Dict::iterator it;
					for (it=DICT(msg.data).begin(); it!=DICT(msg.data).end(); it++)
						cout << "\t" << it->first << ": " << *(it->second) << endl;
				}
			}
		}
	} catch (ConnectionError & err){
		cout << "ERREUR: " << err.what() << endl;
	}
	return 0;
}