
#include "FGameManager.hpp"
#include <Constants.hpp>

using namespace net;
 
///Constructor
FGameManager::FGameManager(ClientManager const & parent) : ClientManager(parent)
{}

/**
 *Method sending queries to the server (connected users)
 */
void FGameManager::askConnectedList()
{
	say(MSG::CONNECTED_USERS_LIST, JSON::String(""));
}

/**
 *Method sending friendly game invitation 
 *@param : string the player to whom the invitation is destined
 */
void FGameManager::sendInvitation(std::string const & name)
{
	say(MSG::FRIENDLY_GAME_USERNAME, JSON::String(name));
}

/**
 *Method that handles server queries
 *@param : string type of the message (INT, STR..)
 *@param : JSON::Value the received message
 */
void FGameManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	///handle query for connected users
	if (type == MSG::CONNECTED_USERS_LIST)
		onUserList(LIST(data));
	///handle query for a friendly game
	else if (type == MSG::FRIENDLY_GAME_INVITATION_RESPONSE){
		JSON::Dict const & details = DICT(data);
		std::string const & name = STR(details.get("username"));
		std::string const & answer = STR(details.get("answer"));
		///handle friendly game ok
		if (answer == MSG::FRIENDLY_GAME_INVITATION_ACCEPT)
			onOtherAccept(name);
		///handle friendly game deny
		else if (answer == MSG::FRIENDLY_GAME_INVITATION_DENY)
			onOtherDeny(name);
		///handle user not found
		else if (answer == MSG::USER_NOT_FOUND)
			onUserNotFound(name);
	}
}

