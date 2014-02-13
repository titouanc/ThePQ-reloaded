#include "User.hpp"
#include <json/json.hpp>
#include <lighttest/lighttest.hpp>

TEST(USER)
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

int main(int argc, const char **argv)
{
	TestFunc tests[] = {
		ADDTEST(USER)
	};
	return RUN(tests);
}
