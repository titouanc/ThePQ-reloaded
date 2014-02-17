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


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(gear_serialization),
	};
	return RUN(tests);
}