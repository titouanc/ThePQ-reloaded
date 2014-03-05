#include "NotificationManager.hpp"

using namespace std;

NotificationManager::NotificationManager(net::ClientConnectionManager& connection, UserData& user) : 
	_connection(connection), _user(user)
{
	
}

void NotificationManager::addCallback(pair<string, AbstractCallback*> callback)
{
	_callbacks.insert(callback);
}

void NotificationManager::handleNotification(JSON::Value *notification){
	JSON::Dict message = DICT(notification);
	string messageType = STR(message.get("type")).value();
	map<string, AbstractCallback*>::const_iterator it = _callbacks.find(messageType);
	if (it != _callbacks.end())
	{
		(*(it->second))(message.get("data"));
	}
}

void NotificationManager::loadTeam(JSON::Value const * data)
{
	JSON::Dict const & dict = DICT(data);
	_user.funds = INT(dict.get("funds"));
}
