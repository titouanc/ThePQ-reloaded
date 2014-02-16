#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include "sys/stat.h"

using namespace std;

class User{
private:
	string _username;
	string _password;
public:
	#define USER_PATH "data/users/"
	
	User(const string& username, const string& password);
	User(const JSON::Dict*);
	
	operator JSON::Dict();

	string getUsername() { return _username; }
	void setUsername(const string& username){ _username = username; };
	
	string getPassword() { return _password; }
	void setPassword(const string& password) { _password = password; }
	
	string getUserDirectoryPath();
	void save();
	
	static User* load(string username);

	string getRandomName();
};

#endif
