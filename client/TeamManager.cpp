#include "TeamManager.hpp"

TeamManager::TeamManager(net::ClientConnectionManager &connection, User& user) : 
	_connection(connection), _user(user)
{}


void TeamManager::displayMenu()
{
	Menu _menu;
	_menu.addToDisplay("	- Show list of players");
	_menu.addToDisplay("    - quit to management menu\n");
	int option;
	do
	{
		option = _menu.run();
		switch(option)
		{
			case 1:
				printPlayers();
				break;
			default:
				break;
		}
	}
	while (option != 1);
}

void TeamManager::loadPlayers(){
	JSON::Dict query, data;
	data.set(net::MSG::USERNAME, _user.username);
	query.set("type", net::MSG::PLAYERS_LIST);
	query.set("data",data);
	_connection.send(query);
	
	JSON::Value *serverResponse = _connection.waitForMsg(net::MSG::PLAYERS_LIST);
	JSON::Dict const & received = DICT(serverResponse);
	JSON::List toFill;
	if(ISLIST(received.get("data"))){
		toFill = LIST(received.get("data"));
	}
	_user.players.clear();
	for(size_t i=0; i<toFill.len();++i){
		Player player(DICT(toFill[i]));
		_user.players.push_back(player);
	}
	delete serverResponse;
}

void TeamManager::printPlayers(){
	if (_user.players.size() == 0)
	{
		loadPlayers();
	}
	cout << "================ YOUR PLAYERS ================" << endl;
	for(size_t i =0; i<_user.players.size();++i){
		cout << _user.players[i] << endl; //modif
	}
	cout << "==============================================" << endl;
}
