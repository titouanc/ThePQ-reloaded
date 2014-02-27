#ifndef Team_hpp
#define Team_hpp

#include <json/json.hpp>
#include <model/Player.hpp>
#include <model/Installation.hpp>
#include <string>
#include <vector>
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>

class Team{
private:
    std::string _name;
    std::string _owner;
    int _funds;
	std::vector<Player> _players;
	std::vector<Installation> _installations;
public:
	Team(): _name(gameconfig::UNNAMED_TEAM), _owner(), _funds(gameconfig::STARTING_FUNDS), 
	_players(), _installations(){}

	Team(std::string owner, std::string teamname=""): Team(){
		_owner = owner;
		_name = teamname;
	}
	Team(const Team& other) : _name(other._name), _owner(other._owner), _funds(other._funds), 
	_players(other._players), _installations(other._installations) {}

	Team(const JSON::Dict &json): Team() {
		if(json.hasKey(net::MSG::USERNAME)) 		{_owner = STR(json.get(net::MSG::USERNAME)).value();}
		if(json.hasKey(memory::FUNDS)) 				{_funds = INT(json.get(memory::FUNDS));}
		if(json.hasKey(memory::TEAM_NAME)) 			{_name = STR(json.get(memory::TEAM_NAME)).value();}
	}

	operator JSON::Dict(){
		JSON::Dict ret;
		ret.set(net::MSG::USERNAME,_owner);
		ret.set(memory::FUNDS,_funds);
		ret.set(memory::TEAM_NAME,_name);
		return ret;
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
		for(size_t i = 0; i<_players.size();++i){
			MemoryAccess::save(_players[i]);
		}
		for(size_t i = 0;i<_installations.size();++i){
			MemoryAccess::save(_installations[i]);
		}
	}
	std::string getOwner(){return _owner;}
	void setOwner(std::string owner){_owner=owner;}
	std::string getName(){return _name;}
	int getFunds(){return _funds;}
	void setFunds(int amount){ _funds = amount;}
	std::vector<Player>& getPlayers(){return _players;}
	std::vector<Installation>& getInstallations(){return _installations;}

	void generateBaseSquad(){
		RandomNameGenerator gen;
		for (int i=0; i<gameconfig::MIN_PLAYERS; ++i){
			Player p;
			p.setName(gen.getRandomName());
			p.setMemberID();
			p.setOwner(getOwner());
			Broomstick broom;
			MemoryAccess::loadSkel(broom);
			p.equipBroomstick(broom);
			Jersey jers;
			MemoryAccess::loadSkel(jers);
			p.equipJersey(jers);
			_players.push_back(p);
		}
	}

	void generateBaseInstallations(){
		MemoryAccess::loadSkel(_installations);
		for(size_t i = 0;i<_installations.size();++i){
			_installations[i].setOwner(getOwner());
		}
	}

	void generateStartingTeam(){
		generateBaseSquad();
		generateBaseInstallations();
	}

};



#endif