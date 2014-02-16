#include "User.hpp"

User::User(const string& username, const string& password)
{
	setUsername(username);
	setPassword(password);
}

User::User(const JSON::Dict* json)
{
	setUsername(STR(json->get(net::MSG::USERNAME)).value());
	setPassword(STR(json->get(net::MSG::PASSWORD)).value());
}

User::operator JSON::Dict()
{
	JSON::Dict ret;
	ret.set(net::MSG::USERNAME, _username);
	ret.set(net::MSG::PASSWORD, _password);
	return ret;
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

// TODO add User.delete


string User::getRandomName(){
	srand(time(NULL));
	string res = "Michel Michel";
	JSON::Value* loaded = JSON::load("names.json");
	if (ISDICT(loaded)){
		JSON::Dict names = DICT(loaded);
		if ((ISINT(names.get("FirstNamesNumber")))
			&& ISLIST(names.get("FirstNames"))){
			JSON::List &firstNames = LIST(names.get("FirstNames"));
			res = STR(firstNames[rand()%INT(names.get("FirstNamesNumber"))]).value();
		}	
		if ((ISINT(names.get("LastNamesNumber")))
			&& ISLIST(names.get("LastNames"))){
			JSON::List &lastNames = LIST(names.get("LastNames"));
			res = res + " "+STR(lastNames[rand()%INT(names.get("LastNamesNumber"))]).value();
		}	
	}
	return res;
}
