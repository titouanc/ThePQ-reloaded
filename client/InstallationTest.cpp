#include "../common/lighttest/lighttest.h"
#include "Installation.hpp"
#include <cmath>

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

TEST(upgrade_installation)
	Installation inst(0, 100);
	for (int i = 0; i < 5; ++i){
		inst.upgrade();
		ASSERT(inst.getLevel() == i+1);
		ASSERT(inst.getCurrentValue() == 100*pow(2, i));
	}
ENDTEST()

TEST(downgrade_installation)
	Installation inst(0, 100);
	for (int i = 0; i < 5; ++i){
		inst.upgrade();
	}
	for (int i = 0; i < 4; ++i){
		inst.downgrade();
		cout << "level : " << inst.getLevel() << endl;
		cout << "getCurrentValue : " << inst.getCurrentValue() << endl;
		ASSERT(inst.getValueAtLevel(inst.getLevel())-inst.getValueAtLevel(inst.getLevel()-1) 
				== inst.getDowngradeRefunds()/inst.getRefundRatio())
		ASSERT(inst.getLevel() == 5-i-1);
		ASSERT(inst.getCurrentValue() == 100*pow(2, inst.getLevel()-1));
	}
	ASSERT(inst.getValueAtLevel(inst.getLevel()) == 100);
	inst.downgrade();
	ASSERT(inst.getLevel() == 0);
	ASSERT(inst.getCurrentValue() == 0);
	ASSERT(inst.getValueAtLevel(0) == 0);
ENDTEST()


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(default_constructor),
		ADDTEST(parameters_constructor_and_getters),
		ADDTEST(setters), 
		ADDTEST(upgrade_installation),
		ADDTEST(downgrade_installation)
	};
	return RUN(tests);
}