#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "TcpSocket.hpp"
#include "Listener.hpp"

class NetworkTest : CppUnit::TestCase {
	public:
		void test()
		{
			
		}
};

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  bool hasFailed = runner.run("", false);
  return (int) ! hasFailed;
}
