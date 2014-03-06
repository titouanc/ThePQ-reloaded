#include <model/User.hpp>
#include <json/json.hpp>
#include <lighttest/lighttest.hpp>
#include "MatchManager.hpp"
#include <model/RandomNameGenerator.hpp>
#include <Constants.hpp>
#include <iostream>
#include "Championship.hpp"

TEST(user)
	User user("usertest", "passwdtest");
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	JSON::Dict json = user;
	ASSERT(STR(json.get(net::MSG::USERNAME)).value() == "usertest");
	ASSERT(STR(json.get("password")).value() == "passwdtest");
	user = User(&json);
	ASSERT(user.getUsername() == "usertest");
	ASSERT(user.getPassword() == "passwdtest");
	//~ user.createUser();
	//~ User *test = User::load("usertest");
	//~ ASSERT(user.getUsername() == "usertest");
	//~ ASSERT(user.getPassword() == "passwdtest");
	//~ delete test;
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

TEST(championship)
	User toon("toon");
	User caca("caca");
	User titou("titou");
	User flo("flo");
	User jerome("jerome");
	Championship cs(2); // 2 turns = 4 players
	ASSERT(cs.getUsers().size() == 0);
	ASSERT(cs.isStarted() == false);
	ASSERT(cs.getNbOfUsers() == 4);
	cs.addUser(toon);
	cs.addUser(caca);
	cs.addUser(titou);
	cs.removeUser(caca);
	cs.addUser(flo);
	cs.addUser(jerome);
	ASSERT(cs.isStarted() == true);
	ASSERT(cs.getUsers()[0] == "toon");
	ASSERT(cs.getUsers()[1] == "titou");
	ASSERT(cs.getUsers()[2] == "flo");
	ASSERT(cs.getUsers()[3] == "jerome");
	Schedule* next = cs.nextMatch();
	ASSERT(next->user1 == "toon");
	ASSERT(next->user2 == "titou");
	next->isHappening = true;
	next = cs.nextMatch();
	ASSERT(next->user1 == "flo");
	ASSERT(next->user2 == "jerome");
	next->isHappening = true;
	next = cs.nextMatch();
	ASSERT(next == NULL);
ENDTEST()

int main()
{
	TestFunc tests[] = {
		ADDTEST(user),
		ADDTEST(squad),
		//ADDTEST(matchmanager),
		ADDTEST(randomname),
		ADDTEST(championship)
	};
	return RUN(tests);
}
