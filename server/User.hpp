#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include "../common/json/json.h"

using namespace std;

class User{
private:
	string _username;
	string _passwd;
public:
	#define USER_PATH "data/users/"
	
	User(const string& username, const string& password);
	User(const JSON::Dict*);

	string getUsername() { return _username; }
	void setUsername(const string& username){ _username = username; };
	
	string getPassword() { return _passwd; }
	void setPassword(const string& passwd) { _passwd = passwd; }
	
	static User* load(string username);
};

#endif
