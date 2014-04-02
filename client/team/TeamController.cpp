#include "TeamController.hpp"

/// Constructor
TeamController::TeamController(ClientController const & parent) : ClientController(parent), _selectedPosition(-1) 
{

}

/**
 * Method handling position selection
 * @param int :  position to select
 */
void TeamController::selectPosition(int position){
	_selectedPosition = position;
}

/**
 * Method handling position
 * @return int : position
 */
int TeamController::getSelectedPosition(){
	return _selectedPosition;
}

/**
 * Method handling player position change
 * @param int : id of the player whose poisition to change
 * @param int : new position at which to place the player
 */
void TeamController::putPlayerAtPosition(int player_id, int position){
	JSON::Dict data = {
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::SQUAD_POSITION, JSON::Integer(position) }
	};
	say(net::MSG::PUT_PLAYER_ON_SQUAD_POSITION, data);
}

/**
 * Method handling player swap (inside a squad)
 * @param int : id of the player to swap
 * @param int : position where to place the player
 */
void TeamController::swapPlayers(int player_id, int position){
	JSON::Dict data = {
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::SQUAD_POSITION, JSON::Integer(position) }
	};
	say(net::MSG::SWAP_PLAYERS_SQUAD_POSITION, data);
}	

