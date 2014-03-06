#include "Championship.hpp"
#include <cmath>

using namespace std;

Championship::Championship(size_t nbOfTurns) : _isStarted(false), _nbOfUsers(2<<(nbOfTurns-1))
{
	if (_nbOfUsers < 2 || _nbOfUsers > 32)
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
	if (_isStarted == false && isUserIn(user) == false)
	{
		// TODO check if user is already in championship
		// Put him in championship
		_users.push_back(user.getUsername());
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
				// TODO remove from championship
				_users.erase(_users.begin()+i);
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
				&& abs(difftime(now, _schedules[i].date)) < 3600) // 5minutes offset max
			{
				return &_schedules[i];
			}
		}
	}
	return NULL;
}

void Championship::endMatch(MatchManager & match)
{
	// TODO get winner, get looser and create next match
}

bool Championship::isUserIn(User & user)
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (_users[i] == user.getUsername())
		{
			return true;
		}
	}
	return false;
}