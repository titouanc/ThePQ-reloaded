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
	ASSERT(c.getName() == "SuperBat");
	ASSERT(c.getDescription() == "NoDescription");
	ASSERT(c.getStrengthBonus() == 123);
	ASSERT(c.getPrecisionBonus() == 456);
ENDTEST()


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(gear_serialization),
		ADDTEST(bat_serialization)
	};
	return RUN(tests);
}