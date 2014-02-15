#include "ConnectionManager.hpp"

using namespace std;
using namespace net;

int main(int argc, const char **argv)
{
	SharedQueue<Message> inbox, outbox;

	try {
		ConnectionManager manager(inbox, outbox, "0.0.0.0", 32123);
		manager.start();
		manager.stop();
	} catch (ConnectionFailedException & err){
		cout << "ERREUR: " << err.what() << endl;
	}
	return 0;
}
