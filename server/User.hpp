#include <string>
#include "../common/net.hpp"

using namespace std;

class User{
private:
	string _username;
	Socket _socket;
public:
	string getUsername() { return _username; }
	void setUsername(string username){ _username = username; };

	Socket getSocket() { return _socket; }
	void setSocket(Socket socket) { _socket = socket; }

	string getUserPath() { return "users/"+getUsername(); }
};