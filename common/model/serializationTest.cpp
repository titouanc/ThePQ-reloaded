#include "lighttest/lighttest.hpp"
#include "Gear.hpp"

TEST(gear_serialization)
	Gear g;
	g.setName("SuperBroom");
	g.setDescription("SuperBroomDescription");
	g.setPrice(1299);
	JSON::Dict d = JSON::Dict(g);
	Gear h(d);
	ASSERT(h.getName() == "SuperBroom");
	ASSERT(h.getDescription() == "SuperBroomDescription");
	ASSERT(h.getPrice() == 1299);
ENDTEST()

TEST(bat_serialization)
	Bat b(123, 456);
	b.setName("SuperBat");
	JSON::Dict d = JSON::Dict(b);
	Bat c(d);
	cout << "Bat serialized : " << d << endl;
	ASSERT(c.getName() == "SuperBat");
	ASSERT(c.getDescription() == "NoDescription");
	ASSERT(c.getStrengthBonus() == 123);
	ASSERT(c.getPrecisionBonus() == 456);
ENDTEST()

TEST(jersey_serialization)
	Jersey j;
	j.setName("SuperJersey");
	JSON::Dict d = j;
	Jersey k(d);
	cout << "Jersey serialized : " << d << endl;
	ASSERT(k.getName() == "SuperJersey");
	ASSERT(k.getStrengthBonus() == 0);
ENDTEST()


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(gear_serialization),
		ADDTEST(bat_serialization),
		ADDTEST(jersey_serialization)
	};
	return RUN(tests);
}