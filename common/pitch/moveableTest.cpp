#include <string>
#include "Moveable.hpp"
#include "../lighttest/lighttest.hpp"

TEST(constructor_without_parameters)
	Moveable m;
	ASSERT(m.getName() == "NoName");
	ASSERT(m.getID() == 0);
	ASSERT(m.getSpeed() == 0);
	ASSERT(m.getPosition().x() == 0);
	ASSERT(m.getPosition().y() == 0);
ENDTEST()

TEST(constructor_with_parameters)
	Position mpos(3, 6);
	Moveable m("MoveableName", 123, 201.5, mpos);
	ASSERT(m.getName() == "MoveableName");
	ASSERT(m.getID() == 123);
	ASSERT(m.getSpeed() == 201.5);
	ASSERT(m.getPosition().x() == 3);
	ASSERT(m.getPosition().y() == 6);
ENDTEST()

TEST(setters)
	Position mpos(3, 6);
	Moveable m("MoveableName", 123, 201.5, mpos);
	m.setName("modifiedname");
	m.setID(1);
	m.setSpeed(10.5);
	Position mpos2(100,2);
	m.setPosition(mpos2);
	ASSERT(m.getName() == "modifiedname");
	ASSERT(m.getID() == 1);
	ASSERT(m.getSpeed() == 10.5);
	ASSERT(m.getPosition().x() == 100);
	ASSERT(m.getPosition().y() == 2);
ENDTEST()



int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(constructor_without_parameters),
		ADDTEST(constructor_with_parameters),
		ADDTEST(setters)
	};
	return RUN(tests);
}