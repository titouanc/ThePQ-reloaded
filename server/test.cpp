#include "connectionmanager.h"

using namespace std;

int main(int argc, const char **argv)
{
	SharedQueue<Message> incoming;

	try {
		ConnectionManager manager(incoming);
		manager.start();
		cout << "Launched server on " << manager.ip() << ":" << manager.port() << endl;
		while (manager.isRunning()){
			Message const & msg = incoming.pop();
			cout << "Got message from client #" << msg.peer_id << endl;
			if (ISDICT(msg.data)){
				JSON::Dict const & dict = DICT(msg.data);
				JSON::Dict::const_iterator it;
				for (it=dict.begin(); it!=dict.end(); it++)
					cout << "\t" << it->first << ": " << *(it->second) << endl;
				if (dict.hasKey("__type__") && ISSTR(dict.get("__type__"))){
					if (STR(dict.get("__type__")).value() == "DISCONNECT"){
						manager.stop();
						break;
					}
				}
			}
			delete msg.data;
		}
	} catch (ConnectionError & err){
		cout << "ERREUR: " << err.what() << endl;
	}
	return 0;
}