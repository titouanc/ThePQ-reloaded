#include <lighttest/lighttest.hpp>
#include <toolbox.hpp>
#include <string>

using namespace std;

TEST(strip_string)
	string a = "Bonjour";
	cout << "strip : " << strip(a) << "|END" << endl;
	ASSERT(strip(a) == "Bonjour");
	string b = "Bonjour ";
	cout << "strip : " << strip(b) << "|END" << endl;
	ASSERT(strip(b) == "Bonjour");
	ASSERT(b == "Bonjour ")
	string c = "Bonjour      ";
	cout << "strip : " << strip(c) << "|END" << endl;
	ASSERT(strip(c) == "Bonjour");
	string d = "   Bonjour ";
	cout << "strip : " << strip(d) << "|END" << endl;
	ASSERT(strip(d) == "Bonjour");
	string e = " ";
	cout << "strip : " << strip(e) << "|END" << endl;
	ASSERT(strip(e) == "");
	string f = "   B onjou r ";
	cout << "strip : " << strip(f) << "|END" << endl;
	ASSERT(strip(f) == "B onjou r");
	string g = "";
	cout << "strip : " << strip(g) << "|END" << endl;
	ASSERT(strip(g) == "");
ENDTEST()

int main()
{
    TestFunc testSuite[] = {
    	ADDTEST(strip_string),
    };

    return RUN(testSuite);
}