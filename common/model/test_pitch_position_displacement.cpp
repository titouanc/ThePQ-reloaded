#include <lighttest/lighttest.hpp>
#include <model/Position.hpp>
#include <model/Displacement.hpp>
#include <model/Pitch.hpp>
#include <vector>

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
    ASSERT(((JSON::List)p).dumps() == "[2, 7]");
ENDTEST()

TEST(position_from_json)
    Position initial(4, -2);
    JSON::Value *list = JSON::parse(((JSON::List)initial).dumps().c_str());
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
    ASSERT(((JSON::List)d).dumps() == "[[-2, 0], [-1, 1]]");
ENDTEST()

TEST(displacement_from_json)
    Displacement initial;
    initial.addMove(West);
    initial.addMove(NorthWest);

    JSON::Value *parsed = JSON::parse(((JSON::List)initial).dumps().c_str());
    ASSERT(ISLIST(parsed));
    Displacement d(LIST(parsed));
    delete parsed;

    ASSERT(d.length() == initial.length());
    ASSERT(d.count() == initial.count());
ENDTEST()

TEST(displacement_with_higher_speed)
    Displacement d;
    d.addMove(3*Pitch::West);
    ASSERT(d.length() == 3);
    ASSERT(d.position() == 3*Pitch::West);
    ASSERT(d.position(0.5, 6) == 3*Pitch::West);
    ASSERT(d.position(0.75, 6) == 3*Pitch::West);
ENDTEST()

TEST(composite_displacement_with_higher_speed)
    Displacement d;
    d.addMove(3*Pitch::West);
    d.addMove(2*Pitch::SouthWest);
    ASSERT(d.length() == 5);
    
    ASSERT(d.position(0.2) == Pitch::West);
    ASSERT(d.position(0.4) == 2*Pitch::West);
    ASSERT(d.position(0.6) == 3*Pitch::West);
    ASSERT(d.position(0.8) == 3*Pitch::West + Pitch::SouthWest);
    ASSERT(d.position() == 3*Pitch::West + 2*Pitch::SouthWest);

    ASSERT(d.position(0.1, 10) == Pitch::West);
    ASSERT(d.position(0.2, 10) == 2*Pitch::West);
    ASSERT(d.position(0.3, 10) == 3*Pitch::West);
    ASSERT(d.position(0.4, 10) == 3*Pitch::West + Pitch::SouthWest);
    ASSERT(d.position(0.5, 10) == 3*Pitch::West + 2*Pitch::SouthWest);
    ASSERT(d.position(1.0, 10) == 3*Pitch::West + 2*Pitch::SouthWest);
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
    delete fixtures;
ENDTEST()

TEST(pitch)
    Position p1, p2(2, 0);
    Moveable m1(0, 0, p1), m2(0, 0, p2);
    Pitch p(10, 10);
    p.insert(&m1);
    p.insert(&m2);

    ASSERT(p.getAt(0, 0) == &m1);
    ASSERT(p.getAt(2, 0) == &m2);
    ASSERT(m1.getPosition() == Position(0, 0));
    ASSERT(m2.getPosition() == Position(2, 0));

    /* Move m1 from 0,0 to 1,1 */
    p.setAt(1, 1, p.getAt(0, 0));
    ASSERT(p.getAt(1, 1) == &m1);

    ASSERT(m1.getPosition() == Position(0, 0));
    ASSERT(m2.getPosition() == Position(2, 0));
ENDTEST()

TEST(pitch_repr)
    Pitch p(100, 36);
    ASSERT(p.inEllipsis(0, 0));
    ASSERT(p.inEllipsis(-25, 0));
    ASSERT(p.inEllipsis(24, 0));
    ASSERT(p.inEllipsis(0, 8));
    ASSERT(p.inEllipsis(0, -9));

    Position pos;
    Moveable m(0, 0, pos);
    p.insert(&m);

    pos = Position(2, 2);
    Moveable n(0, 0, pos);
    p.insert(&n);
ENDTEST()

TEST(free_position)
    Pitch p(100, 36); 
    Position pos(-48, 0);
    Moveable m(0, 0, pos);
    Position pos2(-47, -1);
    Moveable m2(0, 0, pos2);
    p.insert(&m);
    p.insert(&m2);
    std::vector<Position> free = p.freePositionsAround(pos);
    ASSERT(free.size() == 4);
    ASSERT(free[0].x() == -49);
    ASSERT(free[0].y() == 1);
    ASSERT(free[1].x() == -47);
    ASSERT(free[1].y() == 1);
    ASSERT(free[2].x() == -46);
    ASSERT(free[2].y() == 0);
    ASSERT(free[3].x() == -49);
    ASSERT(free[3].y() == -1);
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
        ADDTEST(displacement_with_higher_speed),
        ADDTEST(composite_displacement_with_higher_speed),
        ADDTEST(composite_displacement),
        ADDTEST(composite_invalid_displacement),
        ADDTEST(pitch),
        ADDTEST(pitch_repr),
        ADDTEST(free_position)
    };

    return RUN(testSuite);
}
