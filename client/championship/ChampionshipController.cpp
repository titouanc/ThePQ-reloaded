#include "ChampionshipController.hpp"

ChampionshipController::ChampionshipController(ClientController const & parent) : ClientController(parent)
{}

void ChampionshipController::loadChampionships()
{
	say(net::MSG::JOINABLE_CHAMPIONSHIPS_LIST, JSON::String(""));
}

void ChampionshipController::joinChampionship(std::string champName)
{
	say(net::MSG::JOIN_CHAMPIONSHIP, JSON::String(champName));
}

void ChampionshipController::joinedChampionship()
{
	say(net::MSG::JOINED_CHAMPIONSHIP, JSON::String(""));
}

void ChampionshipController::leaveCurrentChampionship()
{
	say(net::MSG::LEAVE_CHAMPIONSHIP, JSON::String(""));
}

void ChampionshipController::treatMessage(std::string const & type, JSON::Value const * data)
{
	if (type == net::MSG::JOIN_CHAMPIONSHIP)
	{
		if(ISSTR(data)){
			std::string response = STR(data).value();
			std::string message;
			bool ok=false;

			if(response == net::MSG::ALREADY_IN_CHAMPIONSHIP){
				message =  "Cannot take part in more than one championship at the same time.";
				ok = false;
			}
			else if(response == net::MSG::CHAMPIONSHIP_FULL){
				message = "The championship you tried to join is now full.";
				ok = false;
			}
			else if(response == net::MSG::CHAMPIONSHIP_NOT_FOUND){
				message = "Championship not found.";
				ok = false;
			}
			else if(response == net::MSG::ADDED_TO_CHAMPIONSHIP){
				message = "Added to championship.";
				ok = true;
			}
			this->onJoinChampionship(ok, message);
		}
	}
	else if (type == net::MSG::JOINABLE_CHAMPIONSHIPS_LIST)
	{
		JSON::List & champs = LIST(data);
		_champs.clear();
		for(size_t i = 0; i<champs.len();++i){
			_champs.push_back(Championship(DICT(champs[i])));
		}
		this->onChampionshipsLoad();
	}
	else if(type == net::MSG::LEAVE_CHAMPIONSHIP)
	{
		if(ISSTR(data)){
			std::string response = STR(data).value();
			std::string message;

			bool ok = false;

			if(response == net::MSG::CHAMPIONSHIP_STARTED){
				message = "Your championship started; cannot leave it.";
				ok = false;
			}
			else if(response == net::MSG::NOT_IN_CHAMPIONSHIP){
				message = "You are currently not taking part in any championship.";
				ok = false;
			}
			else if(response == net::MSG::REMOVED_FROM_CHAMPIONSHIP){
				message = "Removed from championship.";
				user().joinedChamp = Championship();
				ok = true;
			}
			this->onLeaveChampionship(ok,message);
		}
	}
	else if(type == net::MSG::JOINED_CHAMPIONSHIP)
	{
		user().joinedChamp = Championship();
		if(ISSTR(data)){
			if(STR(data).value() == net::MSG::CHAMPIONSHIP_NOT_FOUND){}
		}
		else if(ISDICT(data)){
			user().joinedChamp = DICT(data);
		}
		onJoinedChampionship();
	}
}
