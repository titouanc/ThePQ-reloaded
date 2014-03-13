#include "lighttest/lighttest.hpp"
#include "Installation.hpp"
#include "Member.hpp"
#include "Player.hpp"
#include "PosMatrix.hpp"
#include <cmath>
#include <json/Document.hpp>
#include "Team.hpp"
#include <Constants.hpp>
#include "MatchResult.hpp"
#include "User.hpp"
#include "MemoryAccess.hpp"
#include <string>

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

TEST(chaser_document)
	JSON::Document<Chaser> doc;
	
	std::string name;
	doc.with("fixtures/chaser.json", [&](Chaser & chaser){
		name = chaser.getName();
		chaser.setName("Coucougnette");
	});
	ASSERT(name == "Chaser1");

	doc.with("fixtures/chaser.json", [&](Chaser & chaser){
		name = chaser.getName();
		chaser.setName("Chaser1");
	});
	ASSERT(name == "Coucougnette");
ENDTEST()

TEST(player_role)
	Player p;
	ASSERT(p.getRole() == "");

	Beater b;
	ASSERT(b.getRole() == "Beater");

	Chaser c;
	ASSERT(c.getRole() == "Chaser");

	Keeper k;
	ASSERT(k.getRole() == "Keeper");

	Seeker s;
	ASSERT(s.getRole() == "Seeker");
ENDTEST()

int main(){
	TestFunc tests[] = {
		ADDTEST(player_instanciation),
		ADDTEST(beaters_instanciation),
		ADDTEST(collision_score),
		ADDTEST(posmatrix),
		ADDTEST(chaser_document),
		ADDTEST(player_role)
	};
	return RUN(tests);
}
