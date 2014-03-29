#include "TeamManager.hpp"

/// Constructor
TeamManager::TeamManager(ClientManager const & parent) : ClientManager(parent), _selectedPosition(-1) 
{

}

/**
 * Method handling position selection
 * @param int :  position to select
 */
void TeamManager::selectPosition(int position){
	_selectedPosition = position;
}

/**
 * Method handling position
 * @return int : position
 */
int TeamManager::getSelectedPosition(){
	return _selectedPosition;
}

/**
 * Method handling player position change
 * @param int : id of the player whose poisition to change
 * @param int : new position at which to place the player
 */
void TeamManager::putPlayerAtPosition(int player_id, int position){
	JSON::Dict data = {
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::SQUAD_POSITION, JSON::Integer(position) }
	};
	say(net::MSG::PUT_PLAYER_ON_SQUAD_POSITION, data);
}

/**
 * Method allowing for the upgrade of a players ability
 * @param int : id of the player whose ability to be upgraded
 * @param int : id of the ability to be upgraded
*/
void TeamManager::upgradePlayerAbility(int player_id, int ability){
	JSON::Dict data = {
		{net::MSG::PLAYER_ID,JSON::Integer(player_id) },
		{net::MSG::ABILITY, JSON::Integer(ability)}
	};
	say(net::MSG::UPGRADE_PLAYER_ABILITY, data);
}

/**
 * Method handling player swap (inside a squad)
 * @param int : id of the player to swap
 * @param int : position where to place the player
 */
void TeamManager::swapPlayers(int player_id, int position){
	JSON::Dict data = {
		{ net::MSG::PLAYER_ID, JSON::Integer(player_id) },
		{ net::MSG::SQUAD_POSITION, JSON::Integer(position) }
	};
	say(net::MSG::SWAP_PLAYERS_SQUAD_POSITION, data);
}	

