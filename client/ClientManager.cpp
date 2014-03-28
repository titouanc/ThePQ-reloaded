#include "ClientManager.hpp"

/// Constructor
ClientManager::ClientManager(
	net::ClientConnectionManager & connection, 
	UserData & user,
	std::queue<JSON::Dict> & notifications
) : 
_connection(connection), _user(user), _notifications(notifications)
{}

/// Constructor
ClientManager::ClientManager(ClientManager const & other) : 
_connection(other._connection), _user(other._user), 
_notifications(other._notifications)
{}

/**
  *Method handling a connection to the server
  *@return a client connection manager
  */
net::ClientConnectionManager & ClientManager::connection() const 
{
	return _connection;
}

/**
  *Method getting user data
  *@return a reference to the current user
  */
UserData & ClientManager::user() const
{
	return _user;
}

/**
  *Method calculating number of available notifications
  *@return integer representing total number of available notifications
  */
int ClientManager::getNbNotifications() const
{
	return (int)_notifications.size();
}

/**
  *Method getting the notifications
  *@return a queu containing all the notifications
  */
std::queue<JSON::Dict> & ClientManager::notifications() const
{
	return _notifications;
}

/**
  *Method handling communication with the server
  *@param string : type of the message to send
  *@param JSON::Value : message to send
  */
void ClientManager::say(std::string const & type, JSON::Value const & data)
{
	JSON::Dict msg = {
		{"type", JSON::String(type)},
		{"data", data}
	};
	_connection.send(msg);
}

/**
  *Method handling queries from the server
  *@param string : type of the query
  *@param JSON::Value : query to handle
  */
void ClientManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if(type == net::MSG::TEAM_INFOS){
		if(ISDICT((data))){
			onTeamInfo(DICT(data));
		}
	}

	else if (type == net::MSG::PLAYERS_LIST)
	{
		user().players.clear();
		JSON::List const & players = LIST(data);
		for(size_t i=0; i<players.len();++i){
			Player player(DICT(players[i]));
			user().players.push_back(player);
		}
		onPlayersLoad();
	}
	
	//Server response from a net::MSG::CHAMPIONSHIP_MATCH_PENDING notification response (subtle...)
	else if( type == net::MSG::CHAMPIONSHIP_MATCH_STATUS)
	{
		std::string response = STR(data).value();
		if (response == net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW)
			this->onNotificationResponse(true,response,"You withdrawed from your match.\nEvicted from championship.");
		else if (response == net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_WON)
			this->onNotificationResponse(true,response,"Your opponent failed to be ready in time for your championship match, you have won the match.");
		else if (response == net::MSG::CHAMPIONSHIP_MATCH_OPPONENT_WITHDRAW)
			this->onNotificationResponse(true,response,"Your opponent withdrawed from your match, you have won the match.");
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_WAIT)
			this->onNotificationResponse(true,response,"You are readyfor your match.\nThe match will start when your opponent is ready.");
		else if(response == net::MSG::CHAMPIONSHIP_MATCH_START){
			this->onNotificationResponse(true,response,"Your opponent is ready too. Match is starting.");
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
			 	type == net::MSG::CHAMPIONSHIP_MATCH_STATUS_CHANGE ||
				type == net::MSG::CHAMPIONSHIP_STATUS_CHANGE ||
				type == net::MSG::END_OF_MATCH_RAPPORT)
	{
		if (type == net::MSG::MARKET_MESSAGE){
			JSON::Dict const & msg = DICT(data);
			std::string const & msgtype = STR(msg.get("type"));
			/* If we won a sale; reload players */
			if (msgtype == net::MSG::WON_SALE_RAPPORT)
				loadPlayers();
		}
		JSON::Dict notif;
		notif.set("type",JSON::String(type));
		notif.set("data",*data);
		_notifications.push(notif);
	}
}

/**
  *Method retrieving users players
  */
void ClientManager::loadPlayers()
{
	while (!user().isLogged())
		readMessages();
	JSON::Dict data = {
		{ net::MSG::USERNAME, JSON::String(user().username) }
	};
	say(net::MSG::PLAYERS_LIST, data);
}

/**
  *Method parsing querie
  */
void ClientManager::readMessage()
{
	JSON::Value * msg = _connection.popMessage();
    JSON::Dict const & dict = DICT(msg);
    std::string messageType = STR(dict.get("type"));
    ClientManager::treatMessage(messageType, dict.get("data"));
    this->treatMessage(messageType, dict.get("data")); /* virtuelle */
    delete msg;
}

/**
  *Method parsing all available queries and dispatching them to handlers
  */
void ClientManager::readMessages() 
{
    while (_connection.hasMessage())
    	readMessage();
}

/**
  *Method treating available notifications
  */
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
				onMessage(onUnplayedMatch(STR(popped.get("data")).value()));
			}
		}
		else if(type == net::MSG::CHAMPIONSHIP_STATUS_CHANGE){
			onMessage(onChampionshipStatusChange(STR(popped.get("data")).value()));
		}
		else if(type == net::MSG::END_OF_MATCH_RAPPORT){
			onMessage(onMatchRapport(DICT(popped.get("data"))));
		}
	}
}

/**
  *Method handling the end of an auction
  */
std::string ClientManager::onEndOfSale(JSON::Dict const & json)
{
	std::stringstream res;
	res << "\nMessage : a sale has ended." << endl;
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
		res << "You bought player " << INT(json.get(net::MSG::PLAYER_ID)) << " for " << INT(json.get(net::MSG::BID_VALUE)) << "." <<endl;
		if(owner==net::MSG::GENERATED_BY_MARKET)
			res << "This player did not belong to any team. He was free. Like the wind."<<endl;
		else
			res << "This player comes from " << owner << "'s team." << endl;
	}
	res<<endl;
	return res.str();
}

/**
  *Method presenting the final match report
  *@param JSON::Dict : match report to handle
  *@return string final report of the championship
  */
std::string ClientManager::onMatchRapport(JSON::Dict const & json){
	std::stringstream res;
	std::string matchType;
	res << "Match rapport" << endl << endl;
	if(ISBOOL(json.get(net::MSG::CHAMPIONSHIP_MATCH))){
		if(BOOL(json.get(net::MSG::CHAMPIONSHIP_MATCH)) == true)
			matchType = "championship match";
		else
			matchType = "friendly game";
	}
	if(ISBOOL(json.get(net::MSG::WON_MATCH))){
		if(BOOL(json.get(net::MSG::WON_MATCH)) == true)
			res << "Congratulations, you have won the " << matchType << " against ";
		else
			res << "Noob, you have lost the " << matchType << " against ";
	}
	if(ISSTR(json.get(net::MSG::USERNAME))){
		res << STR(json.get(net::MSG::USERNAME)).value();
	}
	res << endl << endl;;
	if(ISINT(json.get(net::MSG::MONEY_GAIN))){
		res << "Money : +" << INT(json.get(net::MSG::MONEY_GAIN)) << " $" << endl;
	}
	if(ISINT(json.get(net::MSG::FAME_WON))){
		std::string delta = "";
		if(INT(json.get(net::MSG::FAME_WON)) > 0) { delta = "+"; }
		res << "Fame  : " << delta << INT(json.get(net::MSG::FAME_WON)) << endl;
	}
	if(ISINT(json.get(net::MSG::AP_WON))){
		res << "AP    : +" << INT(json.get(net::MSG::AP_WON)) << endl;
	}
	return res.str();
}

/**
  *Method handling an unplayed game
  *@param string : message to be handled
  *@return string : result of the 
  */
std::string ClientManager::onUnplayedMatch(std::string const & msg){
	std::stringstream res;
	res << "Message : championship match has ended." << endl;
	if(msg == net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_WON){
		res << "Your opponent failed to be ready in time for your championship match, you have won the match." << endl;
	}
	else if(msg == net::MSG::CHAMPIONSHIP_UNPLAYED_MATCH_LOST){
		res << "You failed to be ready in time for your championship match, you have been evicted from the championship." << endl;
	}
	else if(msg == net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW){
		res << "Your opponent withdrawed from the championship match, you have won the match." << endl;
	}
	else
		res << "Unknown message type." << endl;
	res<<endl;
	return res.str();
}

/**
  *Method handling championship stages
  *@param string : message from server regarding status
  *@return new status of the championship (win/start)
  */
std::string ClientManager::onChampionshipStatusChange(std::string const & msg){
	std::stringstream res;
	if (msg == net::MSG::CHAMPIONSHIP_STARTED){
		res << "\nMessage : the championship you joined has started." << endl;
		res << "You can check the infos about your next championship match in your current championship infos."
			<< "\nYou will receive a notification when it is ready to be played." << endl;
	}
	else if(msg == net::MSG::CHAMPIONSHIP_WON){
		res << "\nMessage : you have won the championship !"<<endl;
		res << "Such very much wow." << endl;
	}
	res << endl;
	return res.str();
}

/**
  *Method quering server with ok to start match token
  */
void ClientManager::readyForMatch(){
	say(net::MSG::CHAMPIONSHIP_MATCH_PENDING_RESPONSE,JSON::String(net::MSG::CHAMPIONSHIP_MATCH_READY));
}

/**
  *Method quering server with <withdraw> from championship token
  */
void ClientManager::withdrawFromMatch(){
	say(net::MSG::CHAMPIONSHIP_MATCH_PENDING_RESPONSE,JSON::String(net::MSG::CHAMPIONSHIP_MATCH_WITHDRAW));
}

/**
  *Method handling invitation agreement from other user
  *@param string : user whose invitation is accepted 
  */
void ClientManager::acceptInvitationFromUser(std::string const & username){
	JSON::Dict data = {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_ACCEPT) }
	};
	say (net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}

/**
  *Method handling refusal from other user
  *@param string : user whose invitation is refused
  */
void ClientManager::denyInvitationFromUser(std::string const & username){
	JSON::Dict data {
		{ "username", JSON::String(username) },
		{ "answer", JSON::String(net::MSG::FRIENDLY_GAME_INVITATION_DENY) }
	};
	say(net::MSG::FRIENDLY_GAME_INVITATION_RESPONSE, data);
}

/**
  *Method loading team info in class atributes
  */
void ClientManager::onTeamInfo(JSON::Dict const & json)
{
	if (ISSTR(json.get(net::MSG::USERNAME)))
		_user.username = STR(json.get(net::MSG::USERNAME)).value();
	
	if (ISSTR(json.get("teamname")))
		_user.teamname = STR(json.get("teamname")).value();
	
	if (ISINT(json.get("funds")))
		_user.funds = INT(json.get("funds"));

	if (ISINT(json.get("activity_points")))
		_user.acPoints = INT(json.get("activity_points"));

	if(ISINT(json.get("fame")))
		_user.fame = INT(json.get("fame"));

	if(ISDICT(json.get(memory::SQUAD))){
		_user.squad = DICT(json.get(memory::SQUAD));
		onSquadUpdated();
	}
	onTeamInfoChange();
}

