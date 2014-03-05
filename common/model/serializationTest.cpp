#include "lighttest/lighttest.hpp"
#include "Gear.hpp"
#include "Player.hpp"

TEST(gear_serialization)
	Gear g;
	g.setName("SuperBroom");
	g.setDescription("SuperBroomDescription");
	g.setPrice(1299);
	JSON::Dict d(g);
	Gear h(d);
	ASSERT(h.getName() == "SuperBroom");
	ASSERT(h.getDescription() == "SuperBroomDescription");
	ASSERT(h.getPrice() == 1299);
ENDTEST()

TEST(bat_serialization)
	Bat b(123, 456);
	b.setName("SuperBat");
	JSON::Dict d(b);
	Bat c(d);
	cout << "Bat serialized : " << d << endl;
	ASSERT(c.getName() == "SuperBat");
	ASSERT(c.getDescription() == "NoDescription");
	ASSERT(c.getStrengthBonus() == 123);
	ASSERT(c.getPrecisionBonus() == 456);
ENDTEST()

TEST(broomstick_serialization)
	Broomstick b;
	b.setName("SuperBroomstick");
	b.setCases(123);
	b.setVelocityBonus(456);
	JSON::Dict d = JSON::Dict(b);
	Broomstick c(d);
	cout << "Bat serialized : " << d << endl;
	ASSERT(c.getName() == "SuperBroomstick");
	ASSERT(c.getDescription() == "NoDescription");
	ASSERT(c.getCases() == 123);
	ASSERT(c.getVelocityBonus() == 456);
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

TEST(player_serialization)
	Player p;
	p.setName("SuperPlayer");
	p.improveStrength(10);
	JSON::Dict d = p;
	cout << "GOT HERE" << endl;
	Player q(d);
	cout << "Player serialized : " << d << endl;
	ASSERT(q.getName() == "SuperPlayer");
	ASSERT(q.getStrength() == 15);
	/*
	ASSERT(q.);
	ASSERT();
	ASSERT();
	ASSERT();
	ASSERT();
	ASSERT();
	ASSERT();
	ASSERT();
	*/
ENDTEST()


int main(){
	TestFunc tests[] = {
		ADDTEST(gear_serialization),
		ADDTEST(bat_serialization),
		ADDTEST(broomstick_serialization),
		ADDTEST(jersey_serialization),
		ADDTEST(player_serialization)
	};
	return RUN(tests);
}
