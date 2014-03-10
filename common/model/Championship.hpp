#ifndef __CHAMPIONSHIP_HPP
#define __CHAMPIONSHIP_HPP

#include <vector>
#include <string>
#include <ctime>
#include <json/json.hpp>
#include "MatchResult.hpp"
#include "User.hpp"


struct Schedule
{
	friend std::ostream& operator<< (std::ostream&, const Schedule&);
	std::string user1;
	std::string user2;
	time_t date;
	bool isHappening;
	std::string statusUser1, statusUser2;
	Schedule(std::string u1, std::string u2, clock_t d) : user1(u1), user2(u2), date(d), isHappening(false), statusUser1("pending"), statusUser2("pending") {}
	Schedule(Schedule const & other){
		user1 = other.user1;
		user2 = other.user2;
		date = other.date;
		isHappening = other.isHappening;
		statusUser1 = other.statusUser1;
		statusUser2 = other.statusUser2;
	}
	Schedule(JSON::Dict const & json)
	{
		if (ISSTR(json.get("user1")))		{ user1 = STR(json.get("user1")).value(); }
		if (ISSTR(json.get("user2")))		{ user1 = STR(json.get("user2")).value(); }
		if (ISINT(json.get("date")))		{ date = INT(json.get("date")); }
		if (ISBOOL(json.get("isHappening"))){ isHappening = BOOL(json.get("isHappening")); }
	}
	operator JSON::Dict()
	{
		JSON::Dict res;
		res.set("user1", user1);
		res.set("user2", user2);
		res.set("date", date);
		res.set("isHappening", isHappening);
		return res;
	}
};



class Championship
{
	friend std::ostream& operator<< (std::ostream&, const Championship&);
public:
	Championship(size_t nbOfTurns = 3,std::string name="");
	~Championship();
	Championship(JSON::Dict const & json);
	operator JSON::Dict();


	void addUser(User & user);
	void removeUser(User & user);
	Schedule* nextMatch();
	void endMatch(MatchResult & result);

	bool isFull() const { return _users.size() == _nbOfUsers; }
	bool isEnded() const { return _isEnded; }
	bool isStarted() const { return _isStarted; }
	std::string getName() const { return _name; }
	size_t getNbOfUsers() { return _nbOfUsers; }
	std::vector<std::string>& getUsers() { return _users; }
	bool isUserIn(std::string username);

private:
	bool _isStarted;
	bool _isEnded;
	std::string _name;
	int _turn;
	size_t _nbOfUsers;
	std::vector<std::string> _users;
	std::vector<Schedule> _schedules;

	void start();
	
};

#endif // __CHAMPIONSHIP_HPP