#include <lighttest/lighttest.hpp>
#include "Match.hpp"

TEST(match_instance)
	Squad s, t;
	Match m(s, t);
	ASSERT(! m.isFinished());

	Displacement move;
	move.addMove(Position(1, -1));
	m.addStroke(1, move);

	JSON::List const & deltas = m.playStrokes();
	ASSERT(deltas.len() == 1);
	ASSERT(ISDICT(deltas[0]));


	JSON::Dict const & d = DICT(deltas[0]);
	ASSERT(ISLIST(d.get("from")));
	ASSERT(ISLIST(d.get("to")));
	ASSERT(ISINT(d.get("mid")));

	Position from(LIST(d.get("from")));
	Position to(LIST(d.get("to")));
	ASSERT(to-from == Position(1, -1));
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc testSuite[] = {
		ADDTEST(match_instance)
	};

	return RUN(testSuite);
}
