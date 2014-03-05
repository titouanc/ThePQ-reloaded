#include "NotificationManager.hpp"

using namespace std;

void NotificationManager::addCallback(pair<string, AbstractCallback*> callback)
{
	_callbacks.insert(callback);
}

void NotificationManager::handleNotification(JSON::Value *notification){
	JSON::Dict const & message = DICT(notification);
	string messageType = STR(message.get("type")).value();
	map<string, AbstractCallback*>::const_iterator it = _callbacks.find(messageType);
	if (it != _callbacks.end())
	{
		(*(it->second))(message.get("data"));
	}
}
