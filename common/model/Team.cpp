#include "Team.hpp"
#include <cmath>

/// Constructor
Team::Team(std::string owner, std::string teamname, int funds, int fame, int acpoints): 
	_owner(owner), 
	_name(teamname), 
	_funds(funds),
	_fame(fame), 
	_acpoints(acpoints),
	_players(), 
	_installations()
{
	pthread_mutex_init(&_changes, NULL);
}

/// Copy constructor
Team::Team(const Team& other) : 
	_owner(other._owner), 
	_name(other._name), 
	_squad(other._squad),
	_funds(other._funds),
	_fame(other._fame), 
	_acpoints(other._acpoints),
	_players(other._players), 
	_installations(other._installations)
{
	pthread_mutex_init(&_changes, NULL);
}

/// Copy constructor
Team::Team(const JSON::Dict &json): Team() {
	if(ISSTR(json.get(net::MSG::USERNAME)))
		_owner = STR(json.get(net::MSG::USERNAME)).value();
	if(ISINT(json.get(memory::FUNDS)))
		_funds = INT(json.get(memory::FUNDS));
	if(ISINT(json.get(memory::AC_POINTS)))
		_acpoints = INT(json.get(memory::AC_POINTS));
	if(ISSTR(json.get(memory::TEAM_NAME)))
		_name = STR(json.get(memory::TEAM_NAME)).value();
	if(ISINT(json.get(memory::FAME)))
		_fame = INT(json.get(memory::FAME));
	if(ISDICT(json.get(memory::SQUAD)))
		_squad = DICT(json.get(memory::SQUAD));
}

/// Destructor
Team::~Team()
{
	for (size_t i = 0; i < _installations.size(); ++i){
		if (_installations[i])
			delete _installations[i];
	}
	pthread_mutex_destroy(&_changes);
}
Team::operator JSON::Dict(){
	JSON::Dict ret;
	ret.set(net::MSG::USERNAME,_owner);
	ret.set(memory::FUNDS,_funds);
	ret.set(memory::AC_POINTS,_acpoints);
	ret.set(memory::TEAM_NAME,_name);
	ret.set(memory::FAME, _fame);
	ret.set(memory::SQUAD, JSON::Dict(_squad));
	return ret;
}
void Team::load(){
	/* _owner has to be initialized to an username else throws a JSON::IOError */
	loadInfos(); //Only loads funds, owner, name (TODO : fame, wins, games, etc.).
	MemoryAccess::load(_installations,_owner);
	MemoryAccess::load(_players,_owner);
}

/// Method handling  the save of a teams attributes
void Team::save(){
	/* saves infos such as owner, funds, etc. and data vectors<> */
	saveInfos(); 
	for(size_t i = 0; i<_players.size();++i){
		MemoryAccess::save(_players[i]);
	}
	for(size_t i = 0;i<_installations.size();++i){
		MemoryAccess::save(_installations[i]);
	}
}
bool Team::removePlayer(int id){
	if (pthread_mutex_lock(&_changes) != 0)
	{
		return false;
	}
	for(size_t i=0;i<_players.size();++i){
		if(_players[i].getMemberID()==id){
			MemoryAccess::removeObject(_players[i]);
			_players.erase(_players.begin()+i);
			pthread_mutex_unlock(&_changes);
			return true;
		}
	}
	pthread_mutex_unlock(&_changes);
	return false;
}
void Team::addPlayer(Player &player){
	if (pthread_mutex_lock(&_changes) != 0)
	{
		return;
	}
	cout<<endl;
	cout<<"!*!*!*!*!*saving player: "<< player<<endl;
	player.setOwner(getOwner());
	MemoryAccess::save(player);
	_players.push_back(player);
	pthread_mutex_unlock(&_changes);
}
Player Team::getPlayer(int id){
	for(size_t i=0;i<_players.size();++i){
		if(_players[i].getMemberID() == id){
			return _players[i];
		}
	}
	throw PlayerNotFoundException();
}

bool Team::upgradeInstallation(size_t i)
{
	if (pthread_mutex_lock(&_changes) != 0)
	{
		return false;
	}
	bool res = false;
	if (i < _installations.size() && _installations[i]->getUpgradeCost() <= _funds)
	{
		_funds -= _installations[i]->getUpgradeCost();
		_installations[i]->upgrade();
		res = true;
		MemoryAccess::save(_installations[i]);
		saveInfos();
	}
	pthread_mutex_unlock(&_changes);
	return res;
}

bool Team::downgradeInstallation(size_t i)
{
	if (pthread_mutex_lock(&_changes) != 0)
	{
		return false;
	}
	bool res = false;
	if (i < _installations.size() && _installations[i]->getLevel() > 0)
	{
		_funds += _installations[i]->getDowngradeRefunds();
		_installations[i]->downgrade();
		res = true;
		MemoryAccess::save(_installations[i]);
		saveInfos();
	}
	pthread_mutex_unlock(&_changes);
	return res;
}

void Team::generateBaseSquad(){
	RandomNameGenerator gen;
	for (int i=0; i<gameconfig::MIN_PLAYERS; ++i){
		Player p;
		p.setName(gen.getRandomName());
		p.setMemberID();
		p.setOwner(getOwner());
		_players.push_back(p);
		*(_squad.players[i]) = p;
		_squad.squad_owner = _owner;
	}
}

void Team::generateBaseInstallations(){
	MemoryAccess::loadSkel(_installations);
	for(size_t i = 0;i<_installations.size();++i){
		_installations[i]->setOwner(getOwner());
	}
}

void Team::generateStartingTeam(){
	generateBaseSquad();
	generateBaseInstallations();
}

void Team::initFame(){
	_fame=gameconfig::STARTING_FAME;
}
void Team::initAcPoints(){
	_acpoints=gameconfig::STARTING_AC_POINTS;
}
void Team::timeUpdate()
{
	for (size_t j = 0; j < _installations.size(); ++j)
	{
		// _funds -= _installations[j]->getMaintenanceCost();
		_funds += _fame/gameconfig::STARTING_FAME*_installations[j]->getIncome(); // fame dependant income
	}
	saveInfos();
}

int Team::level () const {
    int sum(0);
    for (Player it : _players)
        sum += it.level();
    return static_cast<int>(pow(sum, 1.0/_players.size()));
}
