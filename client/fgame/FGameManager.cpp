
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

void FGameManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == MSG::CONNECTED_USERS_LIST)
		onUserList(LIST(data));
	else if (type == MSG::FRIENDLY_GAME_INVITATION_RESPONSE){
		JSON::Dict const & details = DICT(data);
		std::string const & name = STR(details.get("username"));
		std::string const & answer = STR(details.get("answer"));
		if (answer == MSG::FRIENDLY_GAME_INVITATION_ACCEPT)
			onOtherAccept(name);
		else if (answer == MSG::FRIENDLY_GAME_INVITATION_DENY)
			onOtherDeny(name);
		else if (answer == MSG::USER_NOT_FOUND)
			onUserNotFound(name);
	}
	else if (type == MSG::FRIENDLY_GAME_INVITATION)
	{
		onInvite(STR(data).value());
	}
}


void FGameManager::acceptInvitationFromUser(string username){
	JSON::Dict data = {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT) }
	};
	say (net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}

void FGameManager::denyInvitationFromUser(string username){
	JSON::Dict data {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_DENY) }
	};
	say(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}