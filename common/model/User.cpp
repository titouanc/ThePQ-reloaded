#include "User.hpp"
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>

User::User(const string& username, const string& password) : 
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

User* User::load(string username){
	try {
		User* user = new User(username, "");
		*user = MemoryAccess::load(*user);
		return user;
	}
	catch (JSON::IOError e)
	{
		return NULL;
	}
}

void User::save(){
	MemoryAccess::save(*this);
}

void User::loadTeam(){
	_team.setOwner(getUsername());
	_team.load();
}



void User::createUser(){
	// Initialization
	MemoryAccess::save(*this);
	// Installations
	/* TODO : add function in MemoryAccess to load object bases (like the basic 
	installations in data/skel/installations.json */
	JSON::Value *loaded = JSON::load(std::string("data/skel/installations.json").c_str());
	JSON::List & base = LIST(loaded);
	for(size_t i=0;i<base.len();++i){
		Installation inst(DICT(base[i]));
		inst.setOwner(getUsername());
		MemoryAccess::save(inst);
	}
	delete loaded;
	// Players
	JSON::List baseSquad;
	generateBaseSquad(baseSquad);
	for(size_t i=0;i<baseSquad.len();++i){
		Player player(DICT(baseSquad[i]));
		MemoryAccess::save(player);
	}
}




// TODO add User.delete
