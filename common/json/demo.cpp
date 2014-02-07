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
	return 0;
}
