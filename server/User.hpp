#ifndef USER_HPP
#define USER_HPP 

#include <string>
#include <json/json.hpp>
#include <network/ConnectionManager.hpp>
#include "sys/stat.h"
#include "RandomNameGenerator.hpp"
#include <model/Installation.hpp>
#include <model/Player.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <istream>

using namespace std;

class User{
private:
	string _username;
	string _password;
	int _funds;
	vector<Installation> _installations;
public:
	User(const string& username, const string& password);
	User(const JSON::Dict*);
	
	operator JSON::Dict();

	/* Getters / Setters */
	string getUsername() const { return _username; }
	void setUsername(const string& username){ _username = username; };
	string getPassword() const { return _password; }
	void setPassword(const string& password) { _password = password; }
	int getFunds() const {return _funds;}
	void setFunds(int funds) {_funds=funds;}
	int buyStuff(int price);
	void getPayed(int price) {_funds+=price;}
	/* Serializable */
	string getUserDirectoryPath() const;
	void save();

	static User* load(string username);
	
	/* Installations */
	vector<Installation>& getInstallations();
	void saveInstallations();

	void createUser();

	void generateBaseSquad(JSON::List &toFill);

	/* Enqueue a message to send when user gets back online */
	void sendOfflineMsg(JSON::Value const & message) const;
	/* Get all enqueued messages for this user */
	JSON::List getOfflineMsg(void) const;
	/* Erase all enqueued messages for this user */
	bool clearOfflineMsg(void) const;
};

#endif
