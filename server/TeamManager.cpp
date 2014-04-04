#include "TeamManager.hpp"
#include "Constants.hpp"

void TeamManager::putPlayerOnSquadPosition(const JSON::Dict &response, int peer_id){
	int position = 0;
	int member_id = 0;
	if (ISINT(response.get(net::MSG::PLAYER_ID)))
		member_id = INT(response.get(net::MSG::PLAYER_ID));
	if (ISINT(response.get(net::MSG::SQUAD_POSITION)))
		position = INT(response.get(net::MSG::SQUAD_POSITION));
	_users[peer_id]->getTeam().getSquad().putPlayerAtPosition(member_id, position);
	sendTeamInfos(_users[peer_id]->getTeam(), peer_id);
	MemoryAccess::save(_users[peer_id]->getTeam());
}

void TeamManager::swapPlayersOfSquad(const JSON::Dict &response, int peer_id){
	int position = 0;
	int member_id = 0;
	if (ISINT(response.get(net::MSG::PLAYER_ID)))
		member_id = INT(response.get(net::MSG::PLAYER_ID));
	if (ISINT(response.get(net::MSG::SQUAD_POSITION)))
		position = INT(response.get(net::MSG::SQUAD_POSITION));
	_users[peer_id]->getTeam().getSquad().swapPlayers(member_id, position);
	sendTeamInfos(_users[peer_id]->getTeam(), peer_id);
	MemoryAccess::save(_users[peer_id]->getTeam());
}

void TeamManager::upgradePlayerAbility(const JSON::Dict &response, int peer_id){
	int member_id = 0;
	int ability = 0;
	if (ISINT(response.get(net::MSG::PLAYER_ID)))
		member_id = INT(response.get(net::MSG::PLAYER_ID));
	if (ISINT(response.get(net::MSG::ABILITY)))
		ability = INT(response.get(net::MSG::ABILITY));
	int decAC=_users[peer_id]->getTeam().upgradePlayerAbility(member_id,ability);
	_users[peer_id]->getTeam().loseAcPoints(decAC);
	
	sendTeamInfos(_users[peer_id]->getTeam(), peer_id);
	MemoryAccess::save(_users[peer_id]->getTeam());
	cout<<"######################"<<endl;

}