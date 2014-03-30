#include <lighttest/lighttest.hpp>
#include <toolbox.hpp>
#include <string>

using namespace std;

TEST(strip_stripped_string)
	string a = "Bonjour";
	ASSERT(strip(a) == "Bonjour");
ENDTEST()

TEST(rstrip_string)
	string b = "Bonjour ";
	ASSERT(strip(b) == "Bonjour");
	ASSERT(b == "Bonjour "); /* Original string doesn't change */
	b = "Bonjour      ";
	ASSERT(strip(b) == "Bonjour");
ENDTEST()

TEST(lstrip_string)
	string d = "   Bonjour";
	ASSERT(strip(d) == "Bonjour");
ENDTEST()

TEST(strip_empty_string)
	string e = " ";
	ASSERT(strip(e) == "");
ENDTEST()

TEST(strip_string_with_inner_junk)
	string f = "   B onjou r ";
	ASSERT(strip(f) == "B onjou r");
ENDTEST()

int main()
{
    TestFunc testSuite[] = {
    	ADDTEST(strip_stripped_string),
    	ADDTEST(rstrip_string),
    	ADDTEST(lstrip_string),
    	ADDTEST(strip_empty_string),
    	ADDTEST(strip_string_with_inner_junk)
    };

    return RUN(testSuite);
}
