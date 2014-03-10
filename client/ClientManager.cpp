#include "ClientManager.hpp"

net::ClientConnectionManager & ClientManager::connection() const 
{
	return _connection;
}

UserData & ClientManager::user() const
{
	return _user;
}

int ClientManager::getNbNotifications() const
{
	return (int)_notifications.size();
}

std::queue<JSON::Dict> & ClientManager::notifications() const
{
	return _notifications;
}

void ClientManager::say(std::string const & type, JSON::Value const & data)
{
	JSON::Dict msg = {
		{"type", JSON::String(type)},
		{"data", data}
	};
	_connection.send(msg);
}à

void ClientManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::TEAM_INFOS)
	{
		onTeamInfo(DICT(data));
	}
	else if (type == net::MSG::PLAYERS_LIST)
	{
		onPlayersLoad(LIST(data));
	}
	
	//Server response from a net::MSG::CHAMPIONSHIP_MATCH_PENDING notification response (subtle...)
	else if( type == net::MSG::CHAMPIONSHIP_MATCH_STATUS)
	{
		std::string response = STR(data).value();
		if (response == net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW)
			this->onNotificationResponse(true,response,"You \033[32mwithdrawed\033[0m from your match.\n\033[31mEvicted\033[0m from championship.");
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_WAIT)
			this->onNotificationResponse(true,response,"You are \033[32mready\033[0m for your match.\nThe match will start when your opponent is ready.\nPlease wait...");
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_START){
			this->onNotificationResponse(true,response,"Your opponent is \033[32mready\033[0m too. Match is starting.");
			this->onMatchStart();
		}
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_NOT_FOUND){
			this->onNotificationResponse(false,response,"Match not found.");
		}
	}
	//Notifications, wait for user to handle
	else if (	type == net::MSG::MARKET_MESSAGE || 
				type == net::MSG::FRIENDLY_GAME_INVITATION ||
			 	type == net::MSG::CHAMPIONSHIP_MATCH_PENDING ||
			 	type == net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE)
	{
		JSON::Dict notif;
		notif.set("type",JSON::String(type));
		notif.set("data",*data);
		_notifications.push(notif);
	}
}

void ClientManager::loadPlayers()
{
	while (!user().isLogged())
		readMessages();
	JSON::Dict data = {
		{ net::MSG::USERNAME, JSON::String(user().username) }
	};
	say(net::MSG::PLAYERS_LIST, data);
}

void ClientManager::readMessage()
{
	JSON::Value * msg = _connection.popMessage();
    JSON::Dict const & dict = DICT(msg);
    std::string messageType = STR(dict.get("type"));
    ClientManager::treatMessage(messageType, dict.get("data"));
    this->treatMessage(messageType, dict.get("data")); /* virtuelle */
    delete msg;
}

void ClientManager::readMessages() 
{
    while (_connection.hasMessage())
    	readMessage();
}

void ClientManager::handleNotification(){
	if(! _notifications.empty()){
		JSON::Dict popped = _notifications.front();
		_notifications.pop();
		std::string type = STR(popped.get("type")).value();
		if(type == net::MSG::FRIENDLY_GAME_INVITATION)
			onInvite(STR(popped.get("data")).value());
		else if(type == net::MSG::MARKET_MESSAGE)
			onMessage(onEndOfSale(DICT(popped.get("data"))));
		else if(type == net::MSG::CHAMPIONSHIP_MATCH_PENDING){
			onMatchPending();
		}
		else if(type == net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE){
			if(ISSTR((popped.get("data")))) {
				std::string data = STR(popped.get("data")).value();
			}
		}
	}
}

std::string ClientManager::onEndOfSale(JSON::Dict const & json)
{
	std::stringstream res;
	res << "\n\033[36mMessage : a sale has ended.\033[0m" << endl;
	if(STR(json.get("type")).value()==net::MSG::END_OF_OWNED_SALE_RAPPORT){
		if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_NOT_SOLD){
			res << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has not been sold." << endl; 
		}
		else if(STR(json.get(net::MSG::RAPPORT_SALE_STATUS)).value() == net::MSG::PLAYER_SOLD){
			res << "Your player " << INT(json.get(net::MSG::PLAYER_ID)) << " has been sold for " << INT(json.get(net::MSG::BID_VALUE)) 
			<< " to " << STR(json.get(net::MSG::CURRENT_BIDDER)).value() << endl;
		}
	}
	else if(STR(json.get("type")).value()==net::MSG::WON_SALE_RAPPORT){
		std::string owner = STR(json.get(net::MSG::SALE_OWNER)).value();
		res << "You bought player " << INT(json.get(net::MSG::PLAYER_ID)) << " for \33[32m" << INT(json.get(net::MSG::BID_VALUE)) << "\033[0m." <<endl;
		if(owner==net::MSG::GENERATED_BY_MARKET)
			res << "This player did not belong to any team. He was free. Like the wind."<<endl;
		else
			res << "This player comes from " << owner << "'s team." << endl;
	}
	res<<endl;
	return res.str();
}

void ClientManager::readyForMatch(){
	say(net::MSG::CHAMPIONSHIP_MATCH_PENDING_RESPONSE,JSON::String(net::MSG::CHAMPIONSHIP_MATCH_READY));
}

void ClientManager::withdrawFromMatch(){
	say(net::MSG::CHAMPIONSHIP_MATCH_PENDING_RESPONSE,JSON::String(net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW));
}

void ClientManager::acceptInvitationFromUser(string username){
	JSON::Dict data = {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT) }
	};
	say (net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}

void ClientManager::denyInvitationFromUser(string username){
	JSON::Dict data {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_DENY) }
	};
	say(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}

void ClientManager::onPlayersLoad(JSON::List const & players)
{
	user().players.clear();
	for(size_t i=0; i<players.len();++i){
		Player player(DICT(players[i]));
		user().players.push_back(player);
	}
}

void ClientManager::onTeamInfo(UserData const & user)
{
	_user.username = user.username;
	_user.funds = user.funds;
	_user.teamname = user.teamname;
}

ClientManager::ClientManager(
	net::ClientConnectionManager & connection, 
	UserData & user,
	std::queue<JSON::Dict> & notifications
) : 
_connection(connection), _user(user), _notifications(notifications)
{}

ClientManager::ClientManager(ClientManager const & other) : 
_connection(other._connection), _user(other._user), 
_notifications(other._notifications)
{}