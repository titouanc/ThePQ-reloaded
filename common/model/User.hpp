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
/* TODO : user.delete() */

class User{
private:
	std::string _username;
	std::string _password;
	Team _team;
public:
	User(const std::string& username ="", const std::string& password="");
	User(const JSON::Dict*);
	operator JSON::Dict();

	std::string getUsername() {return _username;}
	void setUsername(const std::string& username){ _username = username;};
	std::string getPassword() { return _password; }
	void setPassword(const std::string& password) { _password = password; }
	Team& getTeam(){return _team;}

	void save();
	static User* load(std::string username);
	void loadTeam();
	
	void createUser();
	
	/* Enqueue a message to send when user gets back online */
	void sendOfflineMsg(JSON::Value const & message) const;
	/* Get all enqueued messages for this user */
	JSON::List getOfflineMsg(void) const;
	/* Erase all enqueued messages for this user */
	bool clearOfflineMsg(void) const;
};

#endif
