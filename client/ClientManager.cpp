#include "ClientManager.hpp"

TeamInfo::TeamInfo(JSON::Dict const & json)
{
	if (ISSTR(json.get(net::MSG::USERNAME)))
		username = STR(json.get(net::MSG::USERNAME)).value();
	
	if (ISSTR(json.get("teamname")))
		teamname = STR(json.get("teamname")).value();
	
	if (ISINT(json.get("funds")))
		funds = INT(json.get("funds"));
}

net::ClientConnectionManager & ClientManager::connection() const 
{
	return _connection;
}

UserData & ClientManager::user() const
{
	return _user;
}

std::queue<std::string> & ClientManager::notifications() const
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
}

void ClientManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	cout << "ClientManager::treatMessage " << type << endl;
	if (type == net::MSG::TEAM_INFOS)
	{
		TeamInfo team(DICT(data));
		user().username = team.username;
		user().funds = team.funds;
		onTeamInfo(team);
		cout << " XXXXX " << user().username << endl;
	}
	else if (type == net::MSG::MARKET_MESSAGE)
	{
		onEndOfSale(DICT(data));
	}
	else if (type == net::MSG::PLAYERS_LIST)
	{
		onPlayersLoad(LIST(data));
	}
	else if (type == net::MSG::FRIENDLY_GAME_INVITATION)
	{
		onInvite(STR(data).value());
	}
}

void ClientManager::loadPlayers()
{
	JSON::Dict data;
	data.set(net::MSG::USERNAME, user().username);
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

void ClientManager::onEndOfSale(JSON::Dict const & json)
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
	_notifications.push(res.str());
}

void ClientManager::onPlayersLoad(JSON::List const & players)
{
	user().players.clear();
	for(size_t i=0; i<players.len();++i){
		Player player(DICT(players[i]));
		user().players.push_back(player);
		cout << " ****** RECEIVE PLAYER " << player.getName() << endl;
	}
	cout << " ****** TOTAL: " << user().players.size() << endl;
}

ClientManager::ClientManager(
	net::ClientConnectionManager & connection, 
	UserData & user,
	std::queue<std::string> & notifications
) : 
_connection(connection), _user(user), _notifications(notifications)
{}

ClientManager::ClientManager(ClientManager const & other) : 
_connection(other._connection), _user(other._user), 
_notifications(other._notifications)
{}