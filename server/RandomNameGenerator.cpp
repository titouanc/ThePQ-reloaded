#include "RandomNameGenerator.hpp"
#include <stdlib.h>
#include <time.h>
#include <json/json.hpp>

using namespace std;

RandomNameGenerator::RandomNameGenerator() 
{
	srand(time(NULL));
}

string RandomNameGenerator::getRandomName()
{
	string res = "Michel-Michel Michel";
	JSON::Value *loaded = JSON::load("names.json");
	if (ISDICT(loaded)){
		JSON::Dict const & names = DICT(loaded);
		if (ISLIST(names.get("FirstNames")) && ISLIST(names.get("LastNames"))){
			JSON::List const & first = LIST(names.get("FirstNames"));
			JSON::List const & last  = LIST(names.get("LastNames"));
			size_t i = rand()%first.len();
			size_t j = rand()%last.len();
			if (ISSTR(first[i]) && ISSTR(last[j]))
				res = STR(first[i]).value() + " " + STR(last[j]).value();
		}
	}
	if (loaded)
		delete loaded;
	return res;
}