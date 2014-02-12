#include "User.hpp"

User::User(const string& username, const string& password)
{
	setUsername(username);
	setPassword(password);
}

User::User(const JSON::Dict* json)
{
	setUsername(STR(json->get("username")).value());
	setPassword(STR(json->get("password")).value());
}

User::operator JSON::Dict()
{
	JSON::Dict ret;
	ret.set("username", _username);
	ret.set("password", _password);
	return ret;
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
	json.save(string(USER_PATH + _username + "/user.json").c_str());
}
