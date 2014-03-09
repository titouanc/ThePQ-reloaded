#include "CLI.hpp"

using namespace std;

void CLI::prompt() const 
{
	cout << " > ";
}



void CLI::splashScreen() const
{
	cout << "	             _____ _            ____            " << endl;
	cout << "	            |_   _| |__   ___  |  _ \\ _ __ ___ " << endl;
	cout << "		      | | | '_ \\ / _ \\ | |_) | '__/ _ \\\n ";
	cout << "		      | | | | | |  __/ |  __/| | | (_) |" << endl;
	cout << "		      |_| |_| |_|\\___| |_|   |_|  \\___/ " << endl;
	cout << "		   ___        _     _     _ _ _       _ " << endl;
	cout << "		  / _ \\ _   _(_) __| | __| (_) |_ ___| |__ " << endl;
	cout << "		 | | | | | | | |/ _` |/ _` | | __/ __| '_ \\ " << endl;
	cout << "		 | |_| | |_| | | (_| | (_| | | |_ (__| | | |" << endl;
	cout << "		  \\__\\_ \\__,_|_|\\__,_|\\__,_|_|\\__\\___|_| |_|" << endl;
	cout <<  "        =======||\\    	Just a game, no bullshit!" << endl;
	cout <<  "    ===========|| \\___________________________  ___            |" << endl;
	cout <<  "  =============|| |___________________________==___|>        - * -" << endl;
	cout <<  "    ===========|| /                                            |" << endl;
	cout <<  "        =======||/ 		" << endl;
	cout << "		  _ __ ___   __ _ _ __   __ _  __ _  ___ _ __ " << endl;
	cout << "		 | '_ ` _ \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|" << endl;
	cout << "		 | | | | | | (_| | | | | (_| | (_| |  __/ |" << endl;
	cout << "		 |_| |_| |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|" << endl;
	cout << "   		               	              |___/" << endl;
	cout << "			Welcome to The Pro Quidditch Manager 2014!" << endl;
}

void CLI::goodBye() const 
{
	cout <<  "                 =========================            " << endl;
	cout <<  "   Thank you for playing the Pro Quidditch Manager 2014!" << endl;
	cout <<  "                   See you next time! :)" << endl;
	cout <<  "                 =========================            " << endl;
}
