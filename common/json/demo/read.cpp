#include <iostream>
#include <json.h>

using namespace std;

const char *repr = "{\"fame\": 17.2, \"level\": 15, \"name\": \"Titouan\","
				   " \"players_id\": [13, 17]}";

int main(int argc, const char **argv)
{
	JSON::Value *parsed = JSON::parse(repr);

	if (parsed == NULL){
		cerr << "Impossible de parser " << repr << endl;
		return -1;
	}

	if (ISDICT(parsed)){
		JSON::Dict const & manager = DICT(parsed);

		if (manager.hasKey("name") && ISSTR(manager.get("name"))){
			JSON::String & name = STR(manager.get("name"));
			cout << " name: " << name.value() << endl;
		}

		if (manager.hasKey("level") && ISINT(manager.get("level"))){
			JSON::Integer & level = INT(manager.get("level"));
			cout << "level: " << level.value() << endl;
		}

		if (manager.hasKey("fame") && ISFLOAT(manager.get("fame"))){
			JSON::Float & fame = FLOAT(manager.get("level"));
			cout << " fame: " << fame.value() << endl;
		}

		if (manager.hasKey("players_id") && ISLIST(manager.get("players_id"))){
			JSON::List & players = LIST(manager.get("players_id"));
			for (size_t i=0; i<players.len(); i++){
				if (ISINT(players[i])){
					JSON::Integer & player = INT(players[i]);
					cout << "\tplayer[" << i << "]: " << player.value() << endl;
				}
			}
		}
	}

	delete parsed;

	return 0;
}
