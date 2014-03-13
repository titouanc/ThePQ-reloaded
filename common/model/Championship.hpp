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
		if (ISSTR(json.get("statususer1")))	{ statusUser1 = STR(json.get("statususer1")).value();}
		if (ISSTR(json.get("statususer2")))	{ statusUser2 = STR(json.get("statususer2")).value();}
		if (ISINT(json.get("date")))		{ date = INT(json.get("date")); }
		if (ISBOOL(json.get("isHappening"))){ isHappening = BOOL(json.get("isHappening")); }
	}
	operator JSON::Dict()
	{
		JSON::Dict res;
		res.set("user1", user1);
		res.set("user2", user2);
		res.set("date", date);
		res.set("statususer1",statusUser1);
		res.set("statususer2",statusUser2);
		res.set("isHappening", JSON::Bool(isHappening));
		return res;
	}
	std::string getReadableDate() const {
		std::string dateStr = ctime(&date);
		if(! dateStr.empty())
			dateStr.erase(dateStr.size()-1);
		return dateStr;
	}
};



class Championship
{
	friend std::ostream& operator<< (std::ostream&, const Championship&);
public:
	Championship(size_t nbOfTurns = 3,std::string name="",int cashprize = 0, int fame = 0);
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
	bool areUsersNotified() const { return _usersNotified; }
	int getFame() const { return _fame; }
	int getCashPrize() const { return _cashPrize; }
	void usersNotified() { _usersNotified = true; }
	std::string getName() const { return _name; }
	std::string getWinner() const { return (_isEnded) ? _users[0] : ""; }
	size_t getNbOfUsers() { return _nbOfUsers; }
	std::vector<std::string>& getUsers() { return _users; }
	std::vector<Schedule>& getSchedules() { return _schedules; }
	bool isUserIn(std::string username);
	void start();
	void clearSchedules();

private:
	bool _isStarted;
	bool _isEnded;
	bool _usersNotified;
	std::string _name;
	int _cashPrize;
	int _fame;
	int _turn;
	size_t _nbOfUsers;

	std::vector<std::string> _users;
	std::vector<Schedule> _schedules;
	std::vector<std::vector<std::string>> _turnsResult;

	
	
};

#endif // __CHAMPIONSHIP_HPP