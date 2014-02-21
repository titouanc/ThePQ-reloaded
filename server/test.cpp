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
	ASSERT(STR(json.get("username")).value() == "usertest");
	ASSERT(STR(json.get("password")).value() == "passwdtest");
	user = User(&json);
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	user.save();
	User *test = User::load("usertest");
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	delete test;
ENDTEST()


TEST(squad)
	JSON::Value *json = JSON::load("fixtures/squad1.json");
	ASSERT(ISDICT(json));

	Squad sq(DICT(json));
	delete json;

	for (int i=0; i<7; i++){
		cout << sq.players[i]->getName() << " "
	         << sq.players[i]->getVelocity() << endl;
	}
	
	JSON::Dict const & squad = JSON::Dict(sq);
	cout << squad << endl;
ENDTEST()

/*
TEST(matchmanager)
	JSON::Value *A = JSON::load("fixtures/squad1.json");
	ASSERT(ISDICT(A));
	JSON::Value *B = JSON::load("fixtures/squad2.json");
	ASSERT(ISDICT(B));
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
		ADDTEST(squad),
		//ADDTEST(matchmanager),
		ADDTEST(randomname)
	};
	return RUN(tests);
}
