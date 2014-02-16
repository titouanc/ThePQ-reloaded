#include <string>
#include <stdlib.h>
#include <time.h>
#include <json/json.hpp>

class RandomNameGenerator {
public:
	RandomNameGenerator() {srand(time(NULL)); }
	std::string getRandomName();
};
