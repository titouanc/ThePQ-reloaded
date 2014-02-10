#include "../common/lighttest/lighttest.h"
#include "Installation.hpp"

TEST(default_constructor)
	Installation i;
	ASSERT(i.getLevel() == 0);
	ASSERT(i.getCurrentValue() == 100);
ENDTEST()

TEST(parameters_constructor_and_getters)
	Installation i(3,123);
	ASSERT(i.getLevel() == 3);
	ASSERT(i.getCurrentValue() == 123);
ENDTEST()

TEST(setters)
	Installation i(3,123);
	i.setLevel(5);
	i.setCurrentValue(456);
	ASSERT(i.getLevel() == 5);
	ASSERT(i.getCurrentValue() == 456);
	i.setLevel(100);
	ASSERT(i.getLevel() == 5);
ENDTEST()

TEST(upgrade)
	Installation i(0, 100);
ENDTEST()


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(default_constructor),
		ADDTEST(parameters_constructor_and_getters),
		ADDTEST(setters)
	};
	return RUN(tests);
}