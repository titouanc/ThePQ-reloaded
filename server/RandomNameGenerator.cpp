#include "RandomNameGenerator.hpp"

using namespace std;

string RandomNameGenerator::getRandomName(){
	string res = "Michel Michel";
	JSON::Value* loaded = JSON::load("names.json");
	if (ISDICT(loaded)){
		JSON::Dict names = DICT(loaded);
		if ((ISINT(names.get("FirstNamesNumber")))
			&& ISLIST(names.get("FirstNames"))){
			JSON::List &firstNames = LIST(names.get("FirstNames"));
			res = STR(firstNames[rand()%INT(names.get("FirstNamesNumber"))]).value();
		}	
		if ((ISINT(names.get("LastNamesNumber")))
			&& ISLIST(names.get("LastNames"))){
			JSON::List &lastNames = LIST(names.get("LastNames"));
			res = res + " "+STR(lastNames[rand()%INT(names.get("LastNamesNumber"))]).value();
		}	
	}
	return res;
}