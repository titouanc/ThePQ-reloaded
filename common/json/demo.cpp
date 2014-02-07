#include "json.h"
#include <iostream>

using namespace std;

int main(int argc, const char **argv)
{
	JSON::Dict player;
	player.set("name", JSON::String("Stefoune"));
	player.set("speed", JSON::Float(14.2));
	player.set("level", JSON::Integer(12));
	cout << player.dumps() << endl;

	JSON::Value *parsed = JSON::parse(player.dumps().c_str());
	if (parsed->type() == JSON::Dict_t){
		if (DICT(parsed).hasKey("name") && ISSTR(DICT(parsed).get("name")))
			cout << "Name : " << STR(DICT(parsed).get("name")).value() << endl;
		if (DICT(parsed).hasKey("speed") && ISFLOAT(DICT(parsed).get("speed")))
			cout << "Speed: " << FLOAT(DICT(parsed).get("speed")).value() << endl;
		if (DICT(parsed).hasKey("level") && ISINT(DICT(parsed).get("level")))
			cout << "Level: " << INT(DICT(parsed).get("level")).value() << endl;
	}
	return 0;
}
