#include "ChampionshipManager.hpp"

ChampionshipManager::ChampionshipManager(ClientManager const & parent) : ClientManager(parent)
{}

void ChampionshipManager::loadChampionships()
{
	say(net::MSG::JOINABLE_CHAMPIONSHIPS_LIST, JSON::String(""));
}

void ChampionshipManager::joinChampionship(std::string champName)
{
	say(net::MSG::JOIN_CHAMPIONSHIP, JSON::String(champName));
}

void ChampionshipManager::joinedChampionship()
{
	say(net::MSG::JOINED_CHAMPIONSHIP, JSON::String(""));
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
	else if (type == net::MSG::JOINABLE_CHAMPIONSHIPS_LIST)
	{
		JSON::List & champs = LIST(data);
		_champs.clear();
		for(size_t i = 0; i<champs.len();++i){
			_champs.push_back(Championship(DICT(champs[i])));
		}
		onChampionshipsLoad();
	}
	else if(type == net::MSG::LEAVE_CHAMPIONSHIP)
	{
		onLeaveChampionship(STR(data).value());
	}
	else if(type == net::MSG::JOINED_CHAMPIONSHIP)
	{
		user().joinedChamp = Championship();
		if(ISSTR(data)){
			if(STR(data).value() == net::MSG::CHAMPIONSHIP_NOT_FOUND){
				return;
			}
		}
		else if(ISDICT(data)){
			user().joinedChamp = DICT(data);
		}
		onJoinedChampionship();
	}
}
