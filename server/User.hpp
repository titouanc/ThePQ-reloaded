#include <string>
#include "../common/net.hpp"

using namespace std;

class User{
private:
	string _username;
	TcpSocket _socket;
public:
	string getUsername() { return _username; }
	void setUsername(string username){ _username = username; };

	TcpSocket getSocket() { return _socket; }
	void setSocket(TcpSocket socket) { _socket = socket; }

	string getUserPath() { return "users/"+getUsername(); }
};