#ifndef __CHAMPIONSHIP_HPP
#define __CHAMPIONSHIP_HPP

#include <vector>
#include <string>
#include <ctime>
#include "MatchManager.hpp"
#include <json/json.hpp>

struct Schedule
{
	std::string user1;
	std::string user2;
	time_t date;
	Schedule(std::string u1, std::string u2, clock_t d) : user1(u1), user2(u2), date(d) {}
	Schedule(JSON::Dict const & json)
	{
		if (ISSTR(json.get("user1")))	{ user1 = STR(json.get("user1")).value(); }
		if (ISSTR(json.get("user2")))	{ user1 = STR(json.get("user2")).value(); }
		if (ISINT(json.get("date")))	{ date = INT(json.get("date")); }
	}
	operator JSON::Dict()
	{
		JSON::Dict res;
		res.set("user1", user1);
		res.set("user2", user2);
		res.set("date", INT(date));
		return res;
	}
};

class Championship
{
public:
	Championship(size_t nbOfTurns = 3);
	~Championship();
	Championship(JSON::Dict const & json);
	operator JSON::Dict();

	void addUser(User & user);
	void removeUser(User & user);
	std::vector<std::string> nextMatch();
	void endMatch(MatchManager & match);

private:
	bool _isStarted;
	size_t _nbOfUsers;
	std::vector<std::string> _users;
	std::vector<Schedule> _schedules;

	void start();
	bool isUserIn(User & user);
};

#endif // __CHAMPIONSHIP_HPP