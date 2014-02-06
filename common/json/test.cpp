#include "json.h"
#include "lighttest.h"

TEST(json_int)
	JSON::Integer i(3);
	ASSERT(i.value() == 3);
	ASSERT(i.dumps() == "3");
	ASSERT(i.isNumber());
	ASSERT(i.isInteger());
ENDTEST()

TEST(json_float)
	JSON::Float i(3.14);
	ASSERT(i.value() == 3.14);
	ASSERT(i.dumps() == "3.140000");
	ASSERT(i.isNumber());
	ASSERT(! i.isInteger());
ENDTEST()

TEST(json_str)
	JSON::String s("Je suis un canape qui chante");
	ASSERT(s.value() == "Je suis un canape qui chante");
	ASSERT(s.dumps() == "\"Je suis un canape qui chante\"");
	ASSERT(s.isString());
	ASSERT(! s.isNumber());
ENDTEST()

int main(int argc, const char **argv)
{
	TestFunc testSuite[] = {
		ADDTEST(json_int),
		ADDTEST(json_float),
		ADDTEST(json_str)
	};

	return RUN(testSuite);
}
