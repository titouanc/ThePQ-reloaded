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
		onUserList(LIST(msg.get("data")));
	else if (type == MSG::FRIENDLY_GAME_INVITATION_RESPONSE){
		JSON::Dict const & details = DICT(msg.get("data"));
		std::string const & name = STR(details.get("username"));
		std::string const & answer = STR(details.get("answer"));
		if (answer == MSG::FRIENDLY_GAME_INVITATION_ACCEPT)
			onOtherAccept(name);
		else if (answer == MSG::FRIENDLY_GAME_INVITATION_DENY)
			onOtherDeny(name);
		else if (answer == MSG::USER_NOT_FOUND)
			onUserNotFound(name);
	}
}
