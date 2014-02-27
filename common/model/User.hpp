#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include <json/json.hpp>
#include <network/ConnectionManager.hpp>
#include "sys/stat.h"
#include "RandomNameGenerator.hpp"
#include <model/Installation.hpp>
#include <model/Player.hpp>
#include <model/Team.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <istream>
using namespace std;

class User{
private:
	string _username;
	string _password;
	Team _team;
public:
	User(const string& username ="", const string& password="");
	User(const JSON::Dict*);
	operator JSON::Dict();

	string getUsername() {return _username;}
	void setUsername(const string& username){ _username = username;};
	string getPassword() { return _password; }
	void setPassword(const string& password) { _password = password; }
	Team& getTeam(){return _team;}

	void save();
	static User* load(string username);
	void loadTeam();
	
	void createUser();
	void generateBaseSquad(JSON::List &toFill);
};

#endif
