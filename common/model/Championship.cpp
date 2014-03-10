#include "Championship.hpp"
#include <cmath>
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>

using namespace std;
std::ostream& operator<< (std::ostream& out, const Schedule& sche)
    {
        out << "User1(\033[32m" << sche.user1
            << " - " << sche.statusUser1
            << "\033[0m) User2(\033[32m" << sche.user2
            << " - " << sche.statusUser2
            << "\033[0m) Date(\033[32m" << sche.date << "\033[0m)";
        return out;

    }

std::ostream& operator<< (std::ostream& out, const Championship& champ){
	out << champ._name << " : \033[1m" << champ._users.size() << "/" << champ._nbOfUsers << "\033[0m" << std::endl;
	return out;
}

Championship::Championship(size_t nbOfTurns,std::string name) : _isStarted(false), _isEnded(false), _name(name), _turn(1), _nbOfUsers(2<<(nbOfTurns-1))
{
	if (_nbOfUsers < 2 || _nbOfUsers > 32) //Should never happen (AdminClient verifying it)
	{
		_nbOfUsers = 8;
	}
}

Championship::~Championship()
{}

Championship::Championship(JSON::Dict const & json) : Championship()
{
	if (ISINT(json.get("nbOfUsers")))	{ _nbOfUsers = INT(json.get("nbOfUsers")); }
	if (ISBOOL(json.get("isStarted")))	{ _isStarted = BOOL(json.get("isStarted")); }
	if (ISBOOL(json.get("isEnded")))	{ _isEnded = BOOL(json.get("isEnded")); }
	if (ISSTR(json.get("name")))		{ _name = STR(json.get("name")).value(); }
	if (ISINT(json.get("turn")))		{ _turn = INT(json.get("turn")); }
	if (ISLIST(json.get("users")))
	{
		JSON::List users = LIST(json.get("users"));
		for (size_t i = 0; i < users.len(); ++i)
		{
			if (ISSTR(users[i]))
				_users.push_back(STR(users[i]).value());
		}
	}
	if (ISLIST(json.get("schedules")))
	{
		JSON::List schedules = LIST(json.get("schedules"));
		for (size_t i = 0; i < schedules.len(); ++i)
		{
			if (ISDICT(schedules[i]))
				_schedules.push_back(DICT(schedules[i]));
		}
	}
}

Championship::operator JSON::Dict()
{
	JSON::Dict res;
	res.set("nbOfUsers", _nbOfUsers);
	res.set("isStarted", _isStarted);
	res.set("isEnded", _isEnded);
	res.set("name", _name);
	res.set("turn", _turn);
	JSON::List users;
	for (size_t i = 0; i < _users.size(); ++i)
	{
		users.append(_users[i]);
	}
	res.set("users", users);
	JSON::List schedules;
	for (size_t i = 0; i < _schedules.size(); ++i)
	{
		schedules.append((JSON::Dict)_schedules[i]);
	}
	res.set("schedules", schedules);
	return res;
}

void Championship::addUser(User & user)
{
	if (_isStarted == false && isUserIn(user.getUsername()) == false)
	{
		_users.push_back(user.getUsername());
		MemoryAccess::save(*this);
		if (_users.size() == _nbOfUsers)
		{
			start();
		}
	}
}

void Championship::removeUser(User & user)
{
	if (_isStarted == false)
	{
		for (size_t i = 0; i < _users.size(); ++i)
		{
			if (_users[i] == user.getUsername())
			{
				_users.erase(_users.begin()+i);
				MemoryAccess::save(*this);
				return;
			}
		}
	}
}

void Championship::start()
{
	// Create dates for first matches
	// TODO flexible dates
	time_t timeNow = time(NULL);
	struct tm* date = localtime(&timeNow);
	// date->tm_sec = 0;
	date->tm_min += 1;
	// date->tm_hour = 12;
	// date->tm_yday += 7;
	time_t timeBeg = mktime(date);
	for (size_t i = 0; i < _users.size(); i+=2)
	{
		_schedules.push_back(Schedule(_users[i], _users[i+1], timeBeg));
	}
	_isStarted = true;
}

Schedule* Championship::nextMatch()
{
	if (_isStarted == true)
	{
		for (size_t i = 0; i < _schedules.size(); ++i)
		{
			time_t now = time(NULL);
			if (_schedules[i].isHappening == false 
				&& abs(difftime(now, _schedules[i].date)) < gameconfig::MAX_CHAMP_MATCH_OFFSET) // 5minutes offset max
			{
				return &_schedules[i];
			}
		}
	}
	return NULL;
}

void Championship::endMatch(MatchResult & result)
{
	// erase schedule
	for(size_t i = 0;i<_schedules.size();++i){
		if(_schedules[i].user1 == result.winner || _schedules[i].user2 == result.winner){
			_schedules.erase(_schedules.begin()+i);
		}
	}
	// erase loser
	for(size_t i =0;i<_users.size();++i){
		if(_users[i] == result.loser){
			_users.erase(_users.begin()+i);
		}
	}
	// check if championship ended
	if(_users.size() == 1){
		_isEnded = true;
	}
	// check if turn ended
	else if(_users.size() == _nbOfUsers/(2<<(_turn-1))) { //or _schedules.empty() but less safe
		++_turn;
		time_t currentTime = time(NULL);
		struct tm* date = localtime(&currentTime);
		date->tm_min +=1;
		time_t begin = mktime(date);
		for (size_t i = 0; i < _users.size(); i+=2){
			_schedules.push_back(Schedule(_users[i], _users[i+1], begin));
		}
		MemoryAccess::save(*this);
	}

}

bool Championship::isUserIn(std::string username)
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i] == username)
		{
			return true;
		}
	}
	return false;
}