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

class Team{
private:
    std::string _name;
    std::string _owner;
    int _funds;
    int _fame;
	std::vector<Player> _players;
	std::vector<Installation*> _installations;
public:
	Team(std::string owner = "", std::string teamname=gameconfig::UNNAMED_TEAM): _name(teamname), _owner(owner), _funds(gameconfig::STARTING_FUNDS), _fame(gameconfig::STARTING_FAME), _players(), _installations(){}

	Team(const Team& other) : _name(other._name), _owner(other._owner), _funds(other._funds),
	_fame(other._fame), _players(other._players), _installations(other._installations) {}

	Team(const JSON::Dict &json): Team() {
		if(ISSTR(json.get(net::MSG::USERNAME)))		{_owner = STR(json.get(net::MSG::USERNAME)).value();}
		if(ISINT(json.get(memory::FUNDS))) 			{_funds = INT(json.get(memory::FUNDS));}
		if(ISSTR(json.get(memory::TEAM_NAME)))			{_name = STR(json.get(memory::TEAM_NAME)).value();}
		if(ISINT(json.get(memory::FAME)))				{_fame = INT(json.get(memory::FAME));}
	}
	
	~Team()
	{
		for (size_t i = 0; i < _installations.size(); ++i)
		{
			if (_installations[i])
				delete _installations[i];
		}
	}

	operator JSON::Dict(){
		JSON::Dict ret;
		ret.set(net::MSG::USERNAME,_owner);
		ret.set(memory::FUNDS,_funds);
		ret.set(memory::TEAM_NAME,_name);
		return ret;
	}
	
	void saveInfos(){
		MemoryAccess::save(*this);
	}

	void loadInfos(){
		MemoryAccess::load(*this);
	}
	
	void load(){
		/* _owner has to be initialized to an username else throws a JSON::IOError */
		loadInfos(); //Only loads funds, owner, name (TODO : fame, wins, games, etc.).
		MemoryAccess::load(_installations,_owner);
		MemoryAccess::load(_players,_owner);
	}

	void save(){
		/* saves infos such as owner, funds, etc. and data vectors<> */
		saveInfos(); //Same as loads : only funds, owner, etc.
		for(size_t i = 0; i<_players.size();++i){
			MemoryAccess::save(_players[i]);
		}
		for(size_t i = 0;i<_installations.size();++i){
			MemoryAccess::save(_installations[i]);
		}
	}

	std::string getOwner(){return _owner;}
	void setOwner(std::string owner){_owner=owner;}
	std::string getName() const {return _name;}
	void setName(std::string name){_name=name;}
	int getFunds(){return _funds;}
	void getPayed(int amount){_funds+=amount;}
	void buy(int amount){_funds-=amount;}
	std::vector<Player>& getPlayers(){return _players;}
	std::vector<Installation*>& getInstallations(){return _installations;}

	bool removePlayer(int id){
		for(size_t i=0;i<_players.size();++i){
			if(_players[i].getMemberID()==id){
				MemoryAccess::removeObject(_players[i]);
				_players.erase(_players.begin()+i);
				return true;
			}
		}
		return false;
	}

	void addPlayer(Player &player){
		player.setOwner(getOwner());
		MemoryAccess::save(player);
		_players.push_back(player);
	}

	Player getPlayer(int id){
		for(size_t i=0;i<_players.size();++i){
			if(_players[i].getMemberID() == id){
				return _players[i];
			}
		}
		throw PlayerNotFoundException();
	}


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
			_installations[i]->setOwner(getOwner());
		}
	}

	void generateStartingTeam(){
		generateBaseSquad();
		generateBaseInstallations();
	}
	
	void timeUpdate()
	{
		cout << "-----------team : " << getName() << endl;
		cout << "old team funds : " << getFunds() << endl;
		for (size_t j = 0; j < _installations.size(); ++j)
		{
			cout << _installations[j]->getName() << endl;
			cout << _installations[j]->getMaintenanceCost() << endl;
			_funds -= _installations[j]->getMaintenanceCost();
			_funds += _installations[j]->getIncome();
		}
		cout << "new team funds : " << getFunds() << endl;;
		saveInfos();
	}

};



#endif
