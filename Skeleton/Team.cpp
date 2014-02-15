#include "Team.hpp"
#include "Gear.hpp"
#include "Sponsor.hpp"
#include "Installation.hpp"
#include "Member.hpp"

void Team::createStartingPlayers(){
	Player keeper("keeper");
	Player beater1("beater"), beater2("beater");
	Player chaser1("chaser"), chaser2("chaser");
	Player seeker("seeker");
	addPlayer(keeper);
	addPlayer(beater1);
	addPlayer(beater2);
	addPlayer(chaser1);
	addPlayer(chaser2);
	addPlayer(seeker);
}

bool Team::firePlayer(Player player){
	bool exists = false; 
	for(std::list<Player>::iterator it = _players.begin(); it!=_players.end();it++){
		if(*it == player){
			_players.erase(it);
			exists = true; 
		}
	}
	return exists; 
}

bool Team::fireCoach(Coach coach){
	bool exists = false; 
	for(std::list<Coach>::iterator it = _coaches.begin(); it!=_coaches.end();it++){
		if(*it == coach){
			_coaches.erase(it);
			exists = true; 
		}
	}
	return exists; 
}

void bool Team::newSponsor(Sponsor sponsor){
	_sponsor = sponsor;
}

void Team::addPlayer(Player player){
	if(_players.size()<=MAX_PLAYERS){
		_players.push_back(player);
	}
	else{
		throw "Too many players in team.";
	}
}

void Team::addCoach(Coach coach){
	if(_players.size()<=MAX_COACHES){
		_players.push_back(coach);
	}
	else{
		throw "Too many coaches in team.";
	}
}




