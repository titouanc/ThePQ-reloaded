#include "Championship.hpp"
#include <cmath>
#include <model/MemoryAccess.hpp>
#include <Constants.hpp>


using namespace std;
/// Overload of the ostream operator
std::ostream& operator<< (std::ostream& out, const Schedule& sche)
    {
    	std::string status1, status2;
    	if(sche.isHappening){
    		if(sche.statusUser1 == net::MSG::CHAMPIONSHIP_MATCH_READY){
    			status1 = "ready";
    		}
    		else{
    			status1 = sche.statusUser1;
    		}
    		if(sche.statusUser2 == net::MSG::CHAMPIONSHIP_MATCH_READY){
    			status2 = "ready";
    		}
    		else{
    			status2 = sche.statusUser2;
    		}
    	}
    	else{
    		status1 = "match not happening yet";
    		status2 = "match not happening yet";
    	}
        out << "User1(\033[32m" << sche.user1
            << " - " << status1
            << "\033[0m) User2(\033[32m" << sche.user2
            << " - " << status2
            << "\033[0m) Date(\033[32m" << sche.getReadableDate() << "\033[0m)";
        return out;

    }

/// Overload of the ostream operator
std::ostream& operator<< (std::ostream& out, const Championship& champ){
	out << champ._name << " : \033[1m" << champ._users.size() << "/" << champ._nbOfUsers << "\033[0m" 
		<< " Cashprize(\033[32m" << champ._cashPrize << "\033[0m) Fame(\033[32m" << champ._fame << "\033[0m)";
	if(champ._isStarted){
		out << '\n';
		for(size_t i = 0; i < champ._schedules.size();++i){
			out << champ._schedules[i] << '\n';
		}
	}
	return out;
}

/// Constructor
Championship::Championship(size_t nbOfTurns,std::string name,int cashprize, int fame) : 
_isStarted(false), _isEnded(false), _usersNotified(false), _name(name), _turn(1), _totalTurns(nbOfTurns), _nbOfUsers(2<<(nbOfTurns-1)),
_cashPrize(cashprize), _fame(fame)
{
	if (_nbOfUsers < 2 || _nbOfUsers > 32) //Should never happen (AdminClient verifying it)
	{
		_nbOfUsers = 8;
	}
}

/// Destructor
Championship::~Championship()
{}

/// Constructor
Championship::Championship(JSON::Dict const & json) : Championship()
{
	if (ISINT(json.get("nbOfUsers")))	{ _nbOfUsers = INT(json.get("nbOfUsers")); }
	if (ISBOOL(json.get("isStarted")))	{ _isStarted = BOOL(json.get("isStarted")); }
	if (ISBOOL(json.get("isEnded")))	{ _isEnded = BOOL(json.get("isEnded")); }
	if (ISBOOL(json.get("notified")))	{ _usersNotified = BOOL(json.get("notified"));}
	if (ISINT(json.get("cashprize")))	{ _cashPrize = INT(json.get("cashprize"));}
	if (ISINT(json.get("fame")))		{ _fame = INT(json.get("fame"));}
	if (ISINT(json.get("totalTurns")))	{ _totalTurns = INT(json.get("totalTurns")); }
	if (ISSTR(json.get("name")))		{ _name = STR(json.get("name")).value(); }
	if (ISINT(json.get("turn")))		{ _turn = INT(json.get("turn")); }
	if (ISLIST(json.get("users")))
	{
		JSON::List & users = LIST(json.get("users"));
		for (size_t i = 0; i < users.len(); ++i)
		{
			if (ISSTR(users[i]))
				_users.push_back(STR(users[i]).value());
		}
	}
	if (ISLIST(json.get("schedules")))
	{
		JSON::List & schedules = LIST(json.get("schedules"));
		for (size_t i = 0; i < schedules.len(); ++i)
		{
			if (ISDICT(schedules[i]))
				_schedules.push_back(DICT(schedules[i]));
		}
	}
	if(ISLIST(json.get("turnsResult")))
	{
		JSON::List & turnsRes = LIST(json.get("turnsResult"));
		for(size_t i = 0; i < turnsRes.len();++i)
		{
			if(ISLIST(turnsRes[i]))
			{
				JSON::List & turn = LIST(turnsRes[i]);
				std::vector<std::string> turnUsers;
				for(size_t j = 0; j<turn.len();++j){
					if(ISSTR(turn[j])){
						turnUsers.push_back(STR(turn[j]).value());
					}
				}
				_turnsResult.push_back(turnUsers);
			}
		}
	}
}

Championship::operator JSON::Dict()
{
	JSON::Dict res;
	res.set("nbOfUsers", _nbOfUsers);
	res.set("isStarted", JSON::Bool(_isStarted));
	res.set("isEnded", JSON::Bool(_isEnded));
	res.set("notified",JSON::Bool(_usersNotified));
	res.set("name", _name);
	res.set("turn", _turn);
	res.set("fame",_fame);
	res.set("cashprize",_cashPrize);
	res.set("totalTurns",_totalTurns);
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
	JSON::List turnsRes;
	for(size_t i = 0; i < _turnsResult.size(); ++i)
	{
		JSON::List turn;
		for(size_t j = 0; j < _turnsResult[i].size(); ++j)
		{
			turn.append(_turnsResult[i][j]);
		}
		turnsRes.append(turn);
	}
	res.set("turnsResult",turnsRes);
	return res;
}

/**
 * Method adding a user to the championship
 * @param User : user to be added to the championship
 */
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

/**
 * Method removing a user from the championship
 * @param User : user to be removed from the championship
 */
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


/**
 * Method starting a championship
 */
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
	_turnsResult.push_back(_users);
	_isStarted = true;
}

/**
 * Method clearing a championship schedule
 */
void Championship::clearSchedules(){
	_schedules.clear();
}

/**
 * Method handling the schedule for the next match
 * @return Schedule : schedule of the next match
*/
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

/**
 * Method handling the end of a match
 * @return MatchResult : result of the match 
 */
void Championship::endMatch(MatchResult & result)
{
	// erase schedule
	for(size_t i = 0;i<_schedules.size();++i){
		if(_schedules[i].user1 == result.getWinner() || _schedules[i].user2 == result.getWinner()){
			_schedules.erase(_schedules.begin()+i);
		}
	}
	// erase loser
	for(size_t i =0;i<_users.size();++i){
		if(_users[i] == result.getLoser()){
			_users.erase(_users.begin()+i);
		}
	}
	// check if championship ended
	if(_users.size() == 1){
		_isEnded = true;
	}
	// check if turn ended
	else if(_users.size() == _nbOfUsers/(2<<(_turn-1))) { //or _schedules.empty() but less safe
		_turnsResult.push_back(_users);
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

/**
 * Method checking if a user is in the championship
 * @param std::string : username of the user to check
 * @return bool : boolean representing if the user belongs to the championship or not
*/
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