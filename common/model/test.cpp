#include "lighttest/lighttest.hpp"
#include "Installation.hpp"
#include "Member.hpp"
#include "Player.hpp"
#include "PosMatrix.hpp"
#include <cmath>

TEST(default_constructor)
	Installation i;
	ASSERT(i.getLevel() == 0);
	ASSERT(i.getBaseValue() == 100);
	ASSERT(i.getCurrentValue() == 0);
	ASSERT(i.getRefundRatio() == 0.5)
ENDTEST()

TEST(parameters_constructor_and_getters)
	Installation i("name", 123,3,"michel");
	ASSERT(i.getLevel() == 3);
	ASSERT(i.getBaseValue() == 123);
	ASSERT(i.getCurrentValue() == 16*123);
ENDTEST()

TEST(setters)
	Installation i("name", 123,3,"michel");
	i.setLevel(5);
	i.setBaseValue(456);
	ASSERT(i.getLevel() == 5);
	ASSERT(i.getBaseValue() == 456);
	ASSERT(i.getCurrentValue() == 64*456);
	i.setLevel(100);
	ASSERT(i.getLevel() == 5);
ENDTEST()

TEST(upgrade_installation)
	Installation inst("name", 100, 0,"michel");
	for (int i = 0; i < 5; ++i){
		inst.upgrade();
		ASSERT(inst.getLevel() == i+1);
		ASSERT(inst.getCurrentValue() == 100*pow(2, inst.getLevel()+1));
	}
ENDTEST()

TEST(downgrade_installation)
	Installation inst("name", 100, 0,"michel");
	inst.upgrade();
	ASSERT(inst.getDowngradeRefunds() == 200);
	for (int i = 0; i < 4; ++i){
		inst.upgrade();
	}
	for (int i = 0; i < 4; ++i)
	{
		inst.downgrade();
		ASSERT(inst.getValueAtLevel(inst.getLevel())-inst.getValueAtLevel(inst.getLevel()-1) 
				== inst.getDowngradeRefunds()/inst.getRefundRatio())
		ASSERT(inst.getLevel() == 5-i-1);
		ASSERT(inst.getCurrentValue() == 100*pow(2, inst.getLevel()+1));
	}
	cout << inst.getLevel() << inst.getBaseValue();
	ASSERT(inst.getValueAtLevel(inst.getLevel()) == 400);
	ASSERT(inst.getDowngradeRefunds() == 200);
	inst.downgrade();
	ASSERT(inst.getLevel() == 0);
	ASSERT(inst.getCurrentValue() == 0);
	ASSERT(inst.getValueAtLevel(0) == 0);
ENDTEST()

TEST(player_instanciation)
	JSON::Value *json = JSON::load("fixtures/chaser.json");
	if (ISDICT(json)){
		Player p(DICT(json));
		cout << JSON::Dict(p) << endl;
	}
	if (json)
		delete json;
ENDTEST()

TEST(beaters_instanciation)
	JSON::Value *json = JSON::load("fixtures/beater.json");
	if (ISDICT(json)){
		Beater b(DICT(json));
		cout << JSON::Dict(b) << endl;
	}
	if (json)
		delete json;
ENDTEST()

TEST(collision_score)
	Player p;
	cout << p.collisionScore() << ", " <<p.collisionScore() << ", "  << p.collisionScore() << endl;
ENDTEST()

TEST(posmatrix)
	PosMatrix<int> matrix;
	Position p(42, 0);
	ASSERT(matrix.getAt(p) == NULL);

	int val;
	matrix.setAt(Position(p), &val);
	ASSERT(matrix.getAt(p) == &val);

	PosMatrix<int>::iterator it = matrix.begin();
	ASSERT(it != matrix.end());
	ASSERT(it->second == &val);

	it++;
	ASSERT(it == matrix.end());


	matrix.setAt(Position(p), NULL);
	ASSERT(matrix.getAt(p) == NULL);
	it = matrix.begin();
	ASSERT(it == matrix.end());
ENDTEST()


int main(int argc, const char **argv){
	TestFunc tests[] = {
		ADDTEST(default_constructor),
		ADDTEST(parameters_constructor_and_getters),
		ADDTEST(setters), 
		ADDTEST(upgrade_installation),
		ADDTEST(downgrade_installation),
		ADDTEST(player_instanciation),
		ADDTEST(beaters_instanciation),
		ADDTEST(collision_score),
		ADDTEST(posmatrix)
	};
	return RUN(tests);
}
