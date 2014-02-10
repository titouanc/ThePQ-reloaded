#include <string>
#include "Moveable.hpp"
#include "../lighttest/lighttest.h"

ADDTEST(constructor_with_parameters)
	Position mpos(3, 6);
	Moveable m("MoveableName", 123, 201.5, mpos);
	ASSERT(m.getName() == "MoveableName");
ENDTEST();



int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(constructor_with_parameters);
	};
	return RUN(tests);
}