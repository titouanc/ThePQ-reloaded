#include "User.hpp"
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>

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
