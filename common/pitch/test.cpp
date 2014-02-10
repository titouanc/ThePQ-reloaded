#include <lighttest.h>
#include "position.h"
#include "displacement.h"

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
    Position initial(4, -2);
    JSON::Value *list = JSON::parse(initial.toJson().dumps().c_str());
    Position p(LIST(list));
    delete list;

    ASSERT(p.x() == initial.x());
    ASSERT(p.y() == initial.y());
ENDTEST()

TEST(position_length)
    ASSERT(West.length() == 1);
    ASSERT(SouthWest.length() == 1);
    ASSERT(NorthWest.length() == 1);
    ASSERT(East.length() == 1);
    ASSERT(SouthEast.length() == 1);
    ASSERT(NorthEast.length() == 1);
    ASSERT((West*2).length() == 2);
ENDTEST()

TEST(position_combili)
	ASSERT(West+East == Position(0, 0));
    ASSERT(2*West + East == West);
ENDTEST()

TEST(position_is_direction)
    ASSERT(West.isDirection());
    ASSERT((2*West).isDirection());
    ASSERT(! (West+NorthWest).isDirection());
ENDTEST()

TEST(displacement)
    Displacement d;
    ASSERT(d.count() == 0);
    d.addMove(West);
    d.addMove(SouthEast);
    ASSERT(d.count() == 2);
ENDTEST()

TEST(displacement_length)
    Displacement d;
    ASSERT(d.length() == 0);
    d.addMove(West);
    ASSERT(d.length() == 1);
    d.addMove(West);
    ASSERT(d.length() == 2);
ENDTEST()

TEST(displacement_position)
    Displacement d;
    d.addMove(West);
    ASSERT(d.position() == West);
    d.addMove(West);
    ASSERT(d.position() == West*2);
    ASSERT(d.position(0.5) == West);
ENDTEST()

TEST(displacement_not_a_direction)
    Displacement d;
    ASSERT_THROWS(NotADirection, d.addMove(Position(3, 7)));
ENDTEST()

TEST(displacement_to_json)
    Displacement d;
    d.addMove(West);
    d.addMove(NorthWest);
    ASSERT(d.toJson().dumps() == "[[-2, 0], [-1, 1]]");
ENDTEST()

TEST(displacement_from_json)
    Displacement initial;
    initial.addMove(West);
    initial.addMove(NorthWest);

    JSON::Value *parsed = JSON::parse(initial.toJson().dumps().c_str());
    ASSERT(ISLIST(parsed));
    Displacement d(LIST(parsed));
    delete parsed;

    ASSERT(d.length() == initial.length());
    ASSERT(d.count() == initial.count());
ENDTEST()

TEST(composite_displacement)
    JSON::Dict *fixtures = (JSON::Dict*) JSON::load("fixtures/composite.json");
    ASSERT(fixtures && ISDICT(fixtures));

    Displacement d(LIST(fixtures->get("displacement")));
    JSON::List const & positions = LIST(fixtures->get("positions"));

    ASSERT(positions.len() == 1+d.length());
    
    for (size_t i=0; i<positions.len(); i++){
        Position expected(LIST(positions[i]));
        double t = (double)i/(double)d.length();
        Position got = d.position(t);
        ASSERT(expected == got);
    }
    delete fixtures;
ENDTEST()

TEST(composite_invalid_displacement)
    JSON::Dict *fixtures = (JSON::Dict*) JSON::load("fixtures/composite_invalid.json");
    ASSERT(fixtures && ISDICT(fixtures));

    ASSERT_THROWS(NotADirection, Displacement d(LIST(fixtures->get("displacement"))));
ENDTEST()

int main(int argc, const char **argv)
{
    TestFunc testSuite[] = {
    	ADDTEST(position),
        ADDTEST(position_to_json),
        ADDTEST(position_from_json),
        ADDTEST(position_length),
    	ADDTEST(position_combili),
        ADDTEST(position_is_direction),
        ADDTEST(displacement),
        ADDTEST(displacement_length),
        ADDTEST(displacement_position),
        ADDTEST(displacement_not_a_direction),
        ADDTEST(displacement_to_json),
        ADDTEST(displacement_from_json),
        ADDTEST(composite_displacement),
        ADDTEST(composite_invalid_displacement)
    };

    return RUN(testSuite);
}
