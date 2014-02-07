
#include <cppunit/extensions/HelperMacros.h>
#include "TcpSocket.hpp"
#include "Listener.hpp"
#include <cstring>
#include <iostream>
using namespace std;

class NetworkTest : public CppUnit::TestCase {
	public:
		void test()
		{
			net::Listener listServ;
			net::TcpSocket serv, cli;
			net::Socket::Status status;
			char* bufServ = new char[50], *bufCli = new char[50];
			
			status = listServ.listen(6666);
			//~ CCPUNIT_ASSERT(status == net::Socket::Status::OK);
			status = listServ.accept(serv);
			//~ CCPUNIT_ASSERT(status == net::Socket::Status::OK);
			status = cli.connect("127.0.0.1", 6666);
			//~ CCPUNIT_ASSERT(status == net::Socket::Status::OK);			
			
			size_t received;
			strcpy(bufCli, "test1");
			cli.send((const void*) bufCli, 5);
			serv.recv(bufServ, 50, received);
			cout << bufServ << endl;;
			
			delete bufServ;
			delete bufCli;
		}
	CPPUNIT_TEST_SUITE(NetworkTest);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(NetworkTest);
