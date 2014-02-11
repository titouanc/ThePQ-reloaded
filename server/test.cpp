#include "connectionmanager.h"

using namespace std;

int main(int argc, const char **argv)
{
	try {
		ConnectionManager manager;
		while (true) manager.mainloop();
	} catch (ConnectionError & err){
		cout << "ERREUR: " << err.what() << endl;
	}
	return 0;
}