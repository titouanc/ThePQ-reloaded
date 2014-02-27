#include "User.hpp"
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>
#include <errno.h>
#include <unistd.h>

User::User(const std::string& username, const std::string& password) : 
_username(username), _password(password), _team(){}


User::User(const JSON::Dict* json) : User() {
	setUsername(STR(json->get(net::MSG::USERNAME)).value());
	setPassword(STR(json->get("password")).value());
}

User::operator JSON::Dict(){
	JSON::Dict ret;
	ret.set(net::MSG::USERNAME, _username);
	ret.set("password", _password);
	return ret;
}

User* User::load(std::string username){
	try {
		User* user = new User(username);
		MemoryAccess::load(*user);
		return user;
	}
	catch (JSON::IOError e){
		return NULL;
	}
}

void User::save(){
	MemoryAccess::save(*this);
}

void User::loadTeam(){
	_team.setOwner(_username);
	_team.load();
}

void User::createUser(){
	MemoryAccess::save(*this);
	_team.setOwner(_username);
	_team.generateStartingTeam();
	_team.save();
}
#define OFFLINE_MSG_FILENAME "offline_messages.json"
void User::sendOfflineMsg(JSON::Value const & message) const
{
	JSON::List messages = getOfflineMsg();
	messages.append(message);
	messages.save(MemoryAccess::getUserDirectory(_username) + OFFLINE_MSG_FILENAME);
}

JSON::List User::getOfflineMsg(void) const
{
	JSON::Value *loaded = NULL;
	try {
		loaded = JSON::load(MemoryAccess::getUserDirectory(_username) + OFFLINE_MSG_FILENAME);
	} catch (JSON::Error & err){}

	JSON::List res = (loaded) ? LIST(loaded) : JSON::List();
	if (loaded)
		delete loaded;
	return res;
}

bool User::clearOfflineMsg(void) const
{
	int r = unlink((MemoryAccess::getUserDirectory(_username) + OFFLINE_MSG_FILENAME).c_str());
	/* delete successes if unlink() returns 0 or if there was no file */
	return (r == 0 || errno == ENOENT);
	/* errno = error number -> ENOENT == "file not found" */
}