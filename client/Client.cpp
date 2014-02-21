#include "Client.hpp"

using namespace std;

Client::Client(std::string host, int port) : _inbox(), _outbox(),
_connectionManager(_inbox, _outbox, host.c_str(), port)
{
	_connectionManager.start();
}

void Client::loginUser(string username, string passwd)
{
	JSON::Dict toSend, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::LOGIN);
	toSend.set("data", credentials);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data;	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == net::MSG::PASSWORD_ERROR)
				{
					delete serverMessage;
					throw WrongPasswordException();
				}
				else if (STR(received.get("data")).value() == net::MSG::USER_NOT_FOUND)
				{
					delete serverMessage;
					throw UserNotFoundException();
				}
			}
		}
	}
	delete serverMessage;
	// User is logged in at this point.
}

void Client::doesUserExist(string username){
	JSON::Dict toSend;
	toSend.set("type", net::MSG::USER_EXISTS);
	toSend.set("data", username);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data; // receiving server response

	if(ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data")) 
				&& STR(received.get("data")).value() == net::MSG::USER_EXISTS){
				throw UserAlreadyExistsException();
			}
		}
	}
}

void Client::registerUser(string username, string passwd)
{
	JSON::Dict toSend, received, credentials;
	credentials.set(net::MSG::USERNAME, username);
	credentials.set(net::MSG::PASSWORD, passwd);
	toSend.set("type", net::MSG::REGISTER);
	toSend.set("data", credentials);
	net::Message msg(0, toSend.clone());
	_outbox.push(msg);

	JSON::Value *serverMessage = _inbox.pop().data;	// receiving server response

	if (ISDICT(serverMessage)){
		JSON::Dict const &received = DICT(serverMessage);
		if (received.hasKey("type") && ISSTR(received.get("type")) 
			&& STR(received.get("type")).value() == net::MSG::STATUS){
			if (received.hasKey("data") && ISSTR(received.get("data"))) {
				if (STR(received.get("data")).value() == net::MSG::USER_EXISTS)
					throw UserAlreadyExistsException();
			}
		}
	}
	// User is registered
}

vector<Installation> Client::getInstallationsList(){
	JSON::Dict query;
	JSON::List toFill;
	query.set("type", net::MSG::INSTALLATIONS_LIST);
	query.set("data", "");
	net::Message msg(0, query.clone());
	_outbox.push(msg);

	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict response = DICT(serverResponse);
		if (ISLIST(response.get("data")))
		{
			toFill = LIST(response.get("data"));
		}
	}
	vector<Installation> vec;
	for (size_t i = 0; i < toFill.len(); ++i)
	{
		vec.push_back(DICT(toFill[i]));
	}
	delete serverResponse;
	return vec;
}

bool Client::upgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_UPGRADE);
	query.set("data", i);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	
	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict const & received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_UPGRADE)
		{
			ret = received.get("data");
		}
	}
	return ret;
}

bool Client::downgradeInstallation(size_t i)
{
	bool ret = false;
	JSON::Dict query;
	query.set("type", net::MSG::INSTALLATION_DOWNGRADE);
	query.set("data", i);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	
	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse))
	{
		JSON::Dict const &received = DICT(serverResponse);
		
		if (ISSTR(received.get("type")) && ISBOOL(received.get("data"))
			&& STR(received.get("type")).value() == net::MSG::INSTALLATION_DOWNGRADE)
		{
			ret = received.get("data");
		}
	}
	delete serverResponse;
	return ret;
}

vector<std::string> Client::getConnectedUsersList(){
	vector<std::string> res;
	JSON::Dict query;
	query.set("type", net::MSG::CONNECTED_USERS_LIST);
	query.set("data", "");
	net::Message msg(0, query.clone());
	_outbox.push(msg);

	JSON::Value *serverResponse = _inbox.pop().data;
	if (ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & connectedUsers = LIST(received.get("data"));
			for (size_t i = 0; i<connectedUsers.len(); ++i)
				res.push_back(STR(connectedUsers[i]));
		}
	}
	return res;
}

std::vector<Sale> Client::updatePlayersOnSale(){
	JSON::Dict query;
	query.set("type", net::MSG::PLAYERS_ON_MARKET_LIST);
	query.set("data", "");
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	JSON::Value *serverResponse = _inbox.pop().data;
	std::vector<Sale> res;
	if (ISDICT(serverResponse)) {
		JSON::Dict & received = DICT(serverResponse);
		if (ISSTR(received.get("type")) && ISLIST(received.get("data"))){
			JSON::List & sales = LIST(received.get("data"));
			for(size_t i = 0; i<sales.len();++i)
				res.push_back(Sale(DICT(sales[i]), Player(DICT(DICT(sales[i]).get(net::MSG::PLAYER)))));
		}
	}
	return res; 
}

void Client::bidOnPlayer(int player_id,std::string username, int value){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::BID_ON_PLAYER_QUERY);
	query.set("data", data);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	JSON::Value *serverResponse = _inbox.pop().data;
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISSTR(received.get("type")) && ISDICT(received.get("data"))){
			std::string res = STR(DICT(received.get("data")).get(net::MSG::SERVER_RESPONSE));
			if(res == net::MSG::BID_VALUE_NOT_UPDATED)
				throw bidValueNotUpdatedException();
			else if(res == net::MSG::BID_TURN_ERROR)
				throw turnException();
			else if(res == net::MSG::BID_ENDED)
				throw bidEndedException();
			else if(res == net::MSG::CANNOT_BID_ON_YOUR_PLAYER)
				throw bidOnYourPlayerException();
			else if(res == net::MSG::LAST_BIDDER)
				throw lastBidderException();
		}
	}
}

void Client::addPlayerOnMarket(int player_id,std::string username, int value){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME,username);
	data.set(net::MSG::PLAYER_ID,player_id);
	data.set(net::MSG::BID_VALUE,value);
	query.set("type", net::MSG::ADD_PLAYER_ON_MARKET_QUERY);
	query.set("data", data);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	JSON::Value *serverResponse = _inbox.pop().data;
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISSTR(received.get("type")) && ISSTR(received.get("data"))){
			std::string res = STR((received.get("data"))).value();
			if(res == net::MSG::PLAYER_ALREADY_ON_MARKET)
				throw playerAlreadyOnMarketException();
		}
	}
}

std::vector<Player> Client::getPlayers(std::string username){//modif
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, username);//modif
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	net::Message msg(0, query.clone());
	_outbox.push(msg);
	JSON::Value *serverResponse = _inbox.pop().data;
	JSON::List toFill;
	if(ISDICT(serverResponse)){
		JSON::Dict received = DICT(serverResponse);
		if(ISLIST(received.get("data"))){
			toFill = LIST(received.get("data"));
		}
	}
	vector<Player> myplayers;
	for(size_t i=0; i<toFill.len();++i){
		Player player(DICT(toFill[i]));
		myplayers.push_back(player);
	}
	delete serverResponse;
	return myplayers;
}
