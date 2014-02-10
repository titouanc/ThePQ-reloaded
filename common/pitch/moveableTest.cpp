#include <string>
#include "Moveable.hpp"
#include "../lighttest/lighttest.h"

ADDTEST(constructor_with_parameters)
	Position mpos(3, 6);
	Moveable m("MoveableName", 123, 201.5, mpos);
	ASSERT(m.getName() == "MoveableName");
ENDTEST();

ADDTEST(setters)
	Position mpos(3, 6);
	Moveable m("MoveableName", 123, 201.5, mpos);
	m.setName("modifiedname");
	m.setID(1);
	m.setSpeed(10.1);
	Position mpos2(100,2);
	m.setPosition(mpos2);
	ASSERT(m.getName() == "modifiedname");
	ASSERT(m.getID() == 1);
	ASSERT(m.getSpeed() == 10.1);
	ASSERT(m.getPosition().x() == 100);
ENDTEST();



int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(constructor_with_parameters);
		ADDTEST(setters);
	};
	return RUN(tests);
}