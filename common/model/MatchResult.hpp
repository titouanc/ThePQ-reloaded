#ifndef __MATCH_RESULT_HPP
#define __MATCH_RESULT_HPP

#include <string>

struct MatchResult
{
	MatchResult(JSON::Dict const & json)
	{
		if (ISSTR(json.get("winner"))) { winner = STR(json.get("winner")).value(); }
		if (ISSTR(json.get("loser"))) { loser = STR(json.get("loser")).value(); }
		if (ISINT(json.get("score1"))) { score[0] = INT(json.get("score1")); }
		if (ISINT(json.get("score2"))) { score[1] = INT(json.get("score2")); }
	}
	std::string winner;
	std::string loser;
	unsigned int score[2];
};

#endif // __MATCH_RESULT_HPP