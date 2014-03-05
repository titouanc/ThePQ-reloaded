#include <string>
#include <model/Moveable.hpp>
#include <lighttest/lighttest.hpp>

TEST(constructor_without_parameters)
	Moveable m;
	ASSERT(m.getID() == 0);
	ASSERT(m.getSpeed() == 0);
	ASSERT(m.getPosition().x() == 0);
	ASSERT(m.getPosition().y() == 0);
ENDTEST()

TEST(constructor_with_parameters)
	Position mpos(3, 6);
	Moveable m(123, 201.5, mpos);
	ASSERT(m.getID() == 123);
	ASSERT(m.getSpeed() == 201.5);
	ASSERT(m.getPosition().x() == 3);
	ASSERT(m.getPosition().y() == 6);
ENDTEST()

TEST(setters)
	Position mpos(3, 6);
	Moveable m(123, 201.5, mpos);
	m.setID(1);
	m.setSpeed(10.5);
	Position mpos2(100,2);
	m.setPosition(mpos2);
	ASSERT(m.getID() == 1);
	ASSERT(m.getSpeed() == 10.5);
	ASSERT(m.getPosition().x() == 100);
	ASSERT(m.getPosition().y() == 2);
ENDTEST()

TEST(serialize)
	Position pos(1, 2);
	Moveable titou(12, 32.1, pos);

	JSON::Dict const & repr = JSON::Dict(titou);
	cout << repr << endl;
	Moveable copy(repr);

	ASSERT(copy.getPosition() == titou.getPosition());
	ASSERT(copy.getSpeed() == titou.getSpeed());
	ASSERT(copy.getID() == titou.getID());
ENDTEST()

int main(){
	TestFunc tests[] = {
		ADDTEST(constructor_without_parameters),
		ADDTEST(constructor_with_parameters),
		ADDTEST(setters),
		ADDTEST(serialize)
	};
	return RUN(tests);
}
