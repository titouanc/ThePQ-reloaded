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

User* User::load(string username)
{
	string fileName = USER_PATH + username + "/user.json";
	return (User*)JSON::load(fileName.c_str());
}

void User::save()
{
	
}
