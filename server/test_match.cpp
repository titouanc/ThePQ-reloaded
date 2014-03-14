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

TEST(match_with_fixtures)
	Squad s, t;
	Match m(s, t);
	ASSERT(! m.isFinished());

	JSON::Value *fixture = JSON::load("fixtures/match_moves.json");
	JSON::List turns = LIST(fixture);

	for (size_t j=0; j<turns.len(); j++){
		JSON::List const & strokes = LIST(turns[j]);
		for (size_t i=0; i<strokes.len(); i++){
			ASSERT(m.addStroke(DICT(strokes[i])));
		}
		JSON::List const & res = m.playStrokes();
		cout << "MDELTA: " << res << endl;
		ASSERT(res.len() >= strokes.len());
	}

	delete fixture;
ENDTEST()

TEST(match_moveables)
	Squad s, t;
	Match m(s, t);
	ASSERT(! m.isFinished());

	JSON::Dict const & res = m.getMoveables();
	ASSERT(res.len() > 0);
	cout << res << endl;
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc testSuite[] = {
		ADDTEST(match_instance),
		ADDTEST(match_with_fixtures),
		ADDTEST(match_moveables)
	};

	return RUN(testSuite);
}
