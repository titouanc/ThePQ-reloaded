#include "ChampionshipManager.hpp"

ChampionshipManager::ChampionshipManager(ClientManager const & parent) : ClientManager(parent)
{}

void ChampionshipManager::loadChampionships()
{
	say(net::MSG::CHAMPIONSHIPS_LIST, JSON::String(""));
}

void ChampionshipManager::joinChampionship(std::string champName)
{
	say(net::MSG::JOIN_CHAMPIONSHIP, JSON::String(champName));
}

void ChampionshipManager::updateCurrentChampionship()
{
	user().joinedChamp = Championship();
	for(size_t i = 0;i<_champs.size();++i){
		if(_champs[i].isUserIn(user().username)){
			user().joinedChamp = _champs[i];
		}
	}
}

void ChampionshipManager::leaveCurrentChampionship()
{
	say(net::MSG::LEAVE_CHAMPIONSHIP, JSON::String(""));
}

void ChampionshipManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::JOIN_CHAMPIONSHIP)
	{
		onJoinChampionship(STR(data).value());
	}
	else if (type == net::MSG::CHAMPIONSHIPS_LIST)
	{
		onChampionshipsLoad(LIST(data));
	}
	else if(type == net::MSG::LEAVE_CHAMPIONSHIP)
	{
		onLeaveChampionship(STR(data).value());
	}
}

void ChampionshipManager::onChampionshipsLoad(JSON::List const & champs){
	_champs.clear();
	for(size_t i = 0; i<champs.len();++i){
		_champs.push_back(Championship(DICT(champs[i])));
	}
}
