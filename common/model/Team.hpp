#ifndef Team_hpp
#define Team_hpp

#include <json/json.hpp>
#include <model/Player.hpp>
#include <model/Installation.hpp>
#include <string>
#include <vector>
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>
#include <Exception.hpp>
#include "RandomNameGenerator.hpp"

class Team{
private:
    std::string _name;
    std::string _owner;
    int _funds;
    int _fame;
	std::vector<Player> _players;
	std::vector<Installation*> _installations;
public:
	Team(std::string owner = "", std::string teamname=gameconfig::UNNAMED_TEAM, int funds = gameconfig::STARTING_FUNDS, int fame = gameconfig::STARTING_FAME);
	Team(const Team& other);
	Team(const JSON::Dict &json);
	~Team();
	operator JSON::Dict();
	void saveInfos(){
		MemoryAccess::save(*this);
	}
	void loadInfos(){
		MemoryAccess::load(*this);
	}
	void load();
	void save();

	std::string getOwner(){return _owner;}
	void setOwner(std::string owner){_owner=owner;}
	std::string getName() const {return _name;}
	void setName(std::string name){_name=name;}
	int getFunds(){return _funds;}
	void getPayed(int amount){_funds+=amount;}
	void buy(int amount){_funds-=amount;}
	std::vector<Player>& getPlayers(){return _players;}
	std::vector<Installation*>& getInstallations(){return _installations;}
	
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);

	bool removePlayer(int id);
	void addPlayer(Player &player);
	Player getPlayer(int id);

	void generateStartingTeam();
	
	void timeUpdate();
private:
	void generateBaseSquad();
	void generateBaseInstallations();
};



#endif
