#include "User.hpp"
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>

User::User(const string& username, const string& password) : _username(username), 
																_password(password), 
																_funds(500000), _installations()
{
	
}


User::User(const JSON::Dict* json)
{
	setUsername(STR(json->get(net::MSG::USERNAME)).value());
	setPassword(STR(json->get("password")).value());
	setFunds(INT(json->get("funds")).value());
}

User::operator JSON::Dict()
{
	JSON::Dict ret;
	ret.set(net::MSG::USERNAME, _username);
	ret.set("password", _password);
	ret.set("funds", _funds);
	return ret;
}

int User::buyStuff(int price){
	if(price>_funds){
		_funds-=price;
		return 0;
	}else return -1;	
}

User* User::load(string username)
{
	try {
		JSON::Dict infos = MemoryAccess::load(memory::USER,username);
		User* user = new User(&infos);
		return user;
	}
	catch (JSON::IOError e)
	{
		return NULL;
	}
}

void User::save()
{
	JSON::Dict tosave = *this;
	MemoryAccess::save(memory::USER,tosave);
}

vector<Installation>& User::getInstallations()
{
	if (_installations.empty())
	{
		JSON::List installationsList = MemoryAccess::loadList(memory::INST_LIST,_username);
		for (size_t i = 0; i < installationsList.len(); ++i)
		{
			_installations.push_back(DICT(installationsList[i]));
		}
	}
	return _installations;
}

void User::saveInstallations()
{
	for(size_t i =0; i<_installations.size();++i){
		JSON::Dict inst = _installations[i];
		MemoryAccess::save(memory::INSTALLATION, inst);
	}
}

 

void User::createUser(){
	// Initialization
	JSON::Dict json = *this;
	MemoryAccess::save(memory::USER,json);
	// Installations
	/* TODO : add function in MemoryAccess to load object bases (like the basic 
	installations in data/skel/installations.json */
	JSON::Value *loaded = JSON::load(std::string("data/skel/installations.json").c_str());
	JSON::List & base = LIST(loaded);
	for(size_t i=0;i<base.len();++i){
		DICT(base[i]).set(net::MSG::USERNAME,getUsername());
		MemoryAccess::save(memory::INSTALLATION,DICT(base[i]));
	}
	delete loaded;
	// Players
	JSON::List baseSquad;
	generateBaseSquad(baseSquad);
	for(size_t i=0;i<baseSquad.len();++i){
		MemoryAccess::save(memory::PLAYER,DICT(baseSquad[i]));
	}
}


void User::generateBaseSquad(JSON::List &toFill){
	RandomNameGenerator gen;
	for (int i=0; i<7; i++){
		Player p;
		p.setName(gen.getRandomName());
		p.setMemberID();
		p.setOwner(getUsername());
		JSON::Value* tmp = JSON::load("data/skel/broomstick.json");
		p.equipBroomstick(DICT(tmp));
		delete tmp;
		tmp = JSON::load("data/skel/jersey.json");
		p.equipJersey(DICT(tmp));
		delete tmp;
		JSON::Dict dict = p;
		toFill.append(DICT(dict.clone()));
	}
}

// TODO add User.delete
