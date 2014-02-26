#ifndef Team_hpp
#define Team_hpp

#include <json/json.hpp>
#include <model/Player.hpp>
#include <model/Installation.hpp>
#include <string>
#include <vector>

/*
TODO Think about :
- Determinate consts values
- Already got a sponsor at start ? Coach ?
- Nbr starting players ? (7?)
- update installations (Fanshop, MedicalCenter, AlimentationService, Stand)
*/

//Consts
std::string UNNAMED_TEAM = "unnamed_";
int MAX_COACHES = 5;
int MAX_PLAYERS = 15;
int STARTING_FUNDS = 0;

class Team{
private:
    std::string _name;
    std::string _owner;
    int _funds;
	std::vector<Player> _players;
	std::vector<Installation> _installations;
public:
	Team(): _name(UNNAMED_TEAM), _owner(), _funds(STARTING_FUNDS), _players(), _installations(){}
	Team(std::string owner, std::string teamname=""): Team(){
		_owner = owner;
		_name = name;
	}
	Team(const JSON::Dict &json) Team() {
		if(json.hasKey(memory::net::MSG::USERNAME)) {_owner = STR(json.get(net::MSG::USERNAME)).value();}
		if(json.hasKey(memory::FUNDS)) 				{_funds = INT(json.get(memory::FUNDS));}
		if(json.hasKey(memory::TEAM_NAME)) 			{_name = STR(json.get(memory::TEAM_NAME)).value();}
	}

	operator JSON::Dict(){
		JSON::Dict ret;
		ret.set(net::MSG::USERNAME,_owner);
		ret.set(memory::FUNDS,_funds);
		ret.set(memory::TEAM_NAME,_name);
	}
	void load(){
		/* _owner has to be initialized to an username else throws a JSON::IOError */
		*this = MemoryAccess::load(*this); //Only loads funds, owner, name (TODO : fame, wins, games, etc.).
		std::vector<Installation> *fillInstallations = new std::vector<Installation>;
		MemoryAccess::load(fillInstallations,_owner);
		_installations = *fillInstallations;
		delete fillInstallations;
		std::vector<Player> *fillPlayers = new std::vector<Player>;
		MemoryAccess::load(fillPlayers,_owner);
		_players = *fillPlayers;
		delete fillPlayers;
	}

	void save(){
		/* saves infos such as owner, funds, etc. and data vectors<> */
		MemoryAccess::save(*this); //Same as loads : only funds, owner, etc.
		for(size_t i = 0; i<_players;++i){
			MemoryAccess::save(_players[i]);
		}
		for(size_t i = 0;i<_installations;++i){
			MemoryAccess::save(_installations[i]);
		}
	
	}
	std::string getOwner(){return _owner};
	std::string getName(){return _name;}
	int getFunds(){return _funds;}


};



#endif
