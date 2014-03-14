#ifndef Team_hpp
#define Team_hpp

#include <json/json.hpp>
#include <model/Player.hpp>
#include <model/Installation.hpp>
#include <model/Squad.hpp>
#include <string>
#include <vector>
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>
#include <Exception.hpp>
#include "RandomNameGenerator.hpp"
#include <pthread.h>

class Team{
private:
    std::string _owner;
    std::string _name;
    Squad _squad;
    int _funds;
    int _fame;
    int _acpoints;//<-activity points
	std::vector<Player> _players;
	std::vector<Installation*> _installations;
	pthread_mutex_t _changes;
public:
	//Team();
	Team(std::string owner = "", std::string teamname=gameconfig::UNNAMED_TEAM, int funds = gameconfig::STARTING_FUNDS, int fame = gameconfig::STARTING_FAME, 
		int acPoints=gameconfig::STARTING_AC_POINTS);
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

	std::string getOwner() const {return _owner;}
	void setOwner(std::string owner){_owner=owner;}
	std::string getName() const {return _name;}
	void setName(std::string name){_name=name;}

	int getFunds() const {return _funds;}
	int getFame() const {return _fame;}
	int getAcPoints() const {return _acpoints;}
	bool fundsAvailble(int amount) { return amount <= _funds;}

	void loseFame(int amount){
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		if (amount>_fame)
			_fame=0;
		else
			_fame-=amount;
		pthread_mutex_unlock(&_changes);
	}

	int loseFunds(int amount){
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return 0;
		}
		if(amount>_funds){
			amount=_funds;
			_funds=0;
		}else{
			_funds-=amount;
		}
		pthread_mutex_unlock(&_changes);
		return amount;
	}
	
	void earnFame(int amount){ 
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		_fame+=amount;
		pthread_mutex_unlock(&_changes);
	}

	void buy(int amount){
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		_funds-=amount;
		pthread_mutex_unlock(&_changes);
	}
	
	void getPayed(int amount){
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		_funds+=amount;
		pthread_mutex_unlock(&_changes);
	}

	void earnAcPoints(int ap) {
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		_acpoints += ap;
		pthread_mutex_unlock(&_changes);
	}

	void loseAcPoints(int ap) {
		if (pthread_mutex_lock(&_changes) != 0)
		{
			return;
		}
		if(ap > _acpoints)
			_acpoints = 0;
		else
			_acpoints -= ap;
		pthread_mutex_unlock(&_changes);
	}

	std::vector<Player>& getPlayers(){return _players;}
	std::vector<Installation*>& getInstallations(){return _installations;}
	Squad& getSquad() { return _squad; }
	
	bool upgradeInstallation(size_t i);
	bool downgradeInstallation(size_t i);
	Tribune* getTribune()
	{
		for (size_t i = 0; i < _installations.size(); ++i)
		{
			if (_installations[i]->getName() == memory::TRIBUNE)
				return dynamic_cast<Tribune*>(_installations[i]);
		}
		return NULL;
	}

	bool removePlayer(int id);
	void addPlayer(Player &player);
	Player getPlayer(int id);

	void generateStartingTeam();
	void initFame();
	void initAcPoints();
	void timeUpdate();

    int level() const;
private:
	void generateBaseSquad();
	void generateBaseInstallations();
};



#endif
