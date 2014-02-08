#include <lighttest.h>
#include "position.h"

static Position West(-2, 0);
static Position SouthWest(-1, -1); 
static Position SouthEast(1, -1);
static Position East(2, 0);
static Position NorthEast(1, 1);
static Position NorthWest(-1, 1);

TEST(position)
    Position p;
    ASSERT(p.x() == 0);
    ASSERT(p.y() == 0);
    ASSERT(West.x() == -2);
ENDTEST()

TEST(position_to_json)
    Position p(2, 7);
    ASSERT(p.toJson().dumps() == "[2, 7]");
ENDTEST()

TEST(position_from_json)
    JSON::Value *list = JSON::parse("[4, -2]");
    Position p(LIST(list));
    delete list;

    ASSERT(p.x() == 4);
    ASSERT(p.y() == -2);
ENDTEST()

TEST(position_length)
    ASSERT(West.length() == 1);
    ASSERT(SouthWest.length() == 1);
    ASSERT(NorthWest.length() == 1);
    ASSERT(East.length() == 1);
    ASSERT(SouthEast.length() == 1);
    ASSERT(NorthEast.length() == 1);
    ASSERT((2*West).length() == 2);
ENDTEST()

TEST(position_combili)
	ASSERT(West+East == Position(0, 0));
    ASSERT(2*West + East == West);
ENDTEST()

TEST(displacement)
    Displacement d;
    ASSERT(d.count() == 0);
    d.addMove(West);
    d.addMove(SouthEast);
    ASSERT(d.count() == 2);
    ASSERT(d.finalPosition() == SouthWest);
ENDTEST()

TEST(displacement_length)
    Displacement d;
    ASSERT(d.length() == 0);
    d.addMove(West);
    ASSERT(d.length() == 1);
    d.addMove(West);
    ASSERT(d.length() == 2);
ENDTEST()

int main(int argc, const char **argv)
{
    TestFunc testSuite[] = {
    	ADDTEST(position),
        ADDTEST(position_length),
        ADDTEST(position_to_json),
        ADDTEST(position_from_json),
    	ADDTEST(position_combili),
        ADDTEST(displacement),
        ADDTEST(displacement_length)
    };

    return RUN(testSuite);
}
