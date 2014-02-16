#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include <json/json.hpp>
#include <network/TcpSocket.hpp>
#include "sys/stat.h"
#include "RandomNameGenerator.hpp"
#include <model/Installation.hpp>

using namespace std;

class User{
private:
	string _username;
	string _password;
	vector<Installation> _installations;
public:
	#define USER_PATH "data/users/"
	
	User(const string& username, const string& password);
	User(const JSON::Dict*);
	
	operator JSON::Dict();

	/* Getters / Setters */
	string getUsername() { return _username; }
	void setUsername(const string& username){ _username = username; };
	string getPassword() { return _password; }
	void setPassword(const string& password) { _password = password; }
	
	/* Serializable */
	string getUserDirectoryPath();
	void save();
	static User* load(string username);
	
	/* Installations */
	vector<Installation>& getInstallations();
};

#endif
