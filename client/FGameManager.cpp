#include "FGameManager.hpp"
#include <Constants.hpp>

using namespace net;

FGameManager::FGameManager(ClientManager const & parent) : ClientManager(parent)
{}

void FGameManager::askConnectedList()
{
	say(MSG::CONNECTED_USERS_LIST, JSON::String(""));
}

void FGameManager::sendInvitation(std::string const & name)
{
	say(MSG::FRIENDLY_GAME_USERNAME, JSON::String(name));
}

void FGameManager::treatMessage(JSON::Dict const & msg)
{
	std::string type = STR(msg.get("type"));
	if (type == MSG::CONNECTED_USERS_LIST)
		onUserList();
}
