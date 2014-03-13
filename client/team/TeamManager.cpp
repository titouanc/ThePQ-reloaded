#include "TeamManager.hpp"

TeamManager::TeamManager(ClientManager const & parent) : ClientManager(parent), _selectedPosition(-1) 
{

}

void TeamManager::selectPosition(int position){
	_selectedPosition = position;
}

int TeamManager::getSelectedPosition(){
	return _selectedPosition;
}

void TeamManager::putPlayerAtPosition(int player_id, int position){
	JSON::Dict data = {
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::SQUAD_POSITION, JSON::Integer(position) }
	};
	say(net::MSG::PUT_PLAYER_ON_SQUAD_POSITION, data);
}

