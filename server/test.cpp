#include "User.hpp"
#include <json/json.hpp>
#include <lighttest/lighttest.hpp>
#include "MatchManager.hpp"
#include "RandomNameGenerator.hpp"

TEST(user)
	User user("usertest", "passwdtest");
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	JSON::Dict json = user;
	ASSERT(STR(json.get("D_U")).value() == "usertest");
	ASSERT(STR(json.get("D_P")).value() == "passwdtest");
	user = User(&json);
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	user.save();
	User *test = User::load("usertest");
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	delete test;
ENDTEST()

/*
TEST(squad)
	JSON::Value *json = JSON::load("fixtures/squad1.json");
	ASSERT(ISDICT(json));

	Squad sq(DICT(json));
	delete json;

	for (size_t i=0; i<7; i++){
		ASSERT(sq.players[i].getName()[0] == 'A'+(char)i);
		ASSERT(sq.players[i].getSpeed() == 7);
		ASSERT(sq.players[i].getPosition() == Position(0, 0));
		ASSERT(sq.players[i].getID() == 0);
	}
ENDTEST()


TEST(matchmanager)
	JSON::Value *A = JSON::load("fixtures/squad1.json");
	ASSERT(ISDICT(A));
	JSON::Value *B = JSON::load("fixtures/squad2.json");
	ASSERT(ISDICT(B));

	MatchManager m(DICT(A), DICT(B));
	delete A;
	delete B;
ENDTEST()
*/
TEST(randomname)
	RandomNameGenerator r;
	for (int i=0; i<10; i++){
		std::string name = r.getRandomName();
		ASSERT(name.length() > 0);
		ASSERT(name != "Michel-Michel Michel");
		if (i > 0)
			cout << ", ";
		cout << name;
	}
	cout << endl;
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc tests[] = {
		ADDTEST(user),
		//ADDTEST(squad),
		//ADDTEST(matchmanager),
		ADDTEST(randomname)
	};
	return RUN(tests);
}
