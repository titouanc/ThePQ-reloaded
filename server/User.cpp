#include "User.hpp"

User::User(const string& username, const string& password) : _username(username), 
																_password(password), 
																_funds(500000), _installations()
{
	
}


User::User(const JSON::Dict* json)
{
	setUsername(STR(json->get("username")).value());
	setPassword(STR(json->get("password")).value());
	setFunds(INT(json->get("funds")).value());
}

User::operator JSON::Dict()
{
	JSON::Dict ret;
	ret.set("username", _username);
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

string User::getUserDirectoryPath(){
	return USER_PATH + getUsername() + "/";
}

User* User::load(string username)
{
	string fileName = USER_PATH + username + "/user.json";
	try {
		JSON::Value* json = JSON::load(fileName.c_str());
		User* user = new User((JSON::Dict*)json);
		delete json;
		return user;
	}
	catch (JSON::IOError e)
	{
		return NULL;
	}
}

void User::save()
{
	JSON::Dict json = *this;
	mkdir(getUserDirectoryPath().c_str(), 0755);
	json.save(string(getUserDirectoryPath() + "user.json").c_str());
}

vector<Installation>& User::getInstallations()
{
	if (_installations.empty())
	{
		string listPath = getUserDirectoryPath() + "installations.json";
		JSON::Value* json = JSON::load(listPath);
		JSON::List installationsList = LIST(json);
		for (size_t i = 0; i < installationsList.len(); ++i)
		{
			_installations.push_back(DICT(installationsList[i]));
		}
		delete json;
	}
	return _installations;
}

void User::saveInstallations()
{
	JSON::List json;
	for (size_t i = 0; i < _installations.size(); ++i)
	{
		JSON::Dict install = _installations[i];
		json.append(install);
	}
	string path = getUserDirectoryPath() + "installations.json";
	cout << json.dumps() << endl;
	json.save(path.c_str());
}

void User::createUser(){
	// Initialization
	JSON::Dict json = *this;
	mkdir("data", 0755);
	mkdir("data/users/", 0755);
	mkdir(getUserDirectoryPath().c_str(), 0755);
	json.save(string(getUserDirectoryPath() + "user.json").c_str());
	// Installations
	fstream f("data/skel/installations.json", fstream::in|fstream::binary);
	f << noskipws;
	istream_iterator<unsigned char> begin(f);
	istream_iterator<unsigned char> end;
	fstream f2(getUserDirectoryPath()+"installations.json", fstream::out|fstream::trunc|fstream::binary);
	ostream_iterator<char> begin2(f2);
	copy(begin, end, begin2);
	// Players
	JSON::List baseSquad;
	generateBaseSquad(baseSquad);
	baseSquad.save(string(getUserDirectoryPath()+ "players.json").c_str());
}


void User::generateBaseSquad(JSON::List &toFill){
	RandomNameGenerator gen;
	for (int i=0; i<7; i++){
		Player p;
		p.setName(gen.getRandomName());
		p.setMemberID();
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
