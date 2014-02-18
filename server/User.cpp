#include "User.hpp"

User::User(const string& username, const string& password) : _installations()
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

// TODO add User.delete
