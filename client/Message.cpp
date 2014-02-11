#include "Message.hpp"

using namespace std;

string Message::splashScreen(){
	string message;
	message+= "           __       _   \n";
	message+= "           //_ _   /_/_ _                  much amazing  |\n";
	message+= "          // //_' /  //_/                              - * -\n";
	message+= "      ____       _     _     _     _ _ _       _         |\n";
	message+= "     /___ \\_   _(_) __| | __| | __| (_) |_ ___| |__  \n";
	message+= "    //  / / | | | |/ _` |/ _` |/ _` | | __/ __| '_ \\ \n";
	message+= "   / \\_/ /| |_| | | (_| | (_| | (_| | | || (__| | | |\n";
	message+= "   \\___,_\\ \\__,_|_|\\__,_|\\__,_|\\__,_|_|\\__\\___|_| |_|\n";
	message+= "        =======||\\\n";
	message+= "    ===========|| \\___________________________  ___            |\n";
	message+= "  =============||  |__________________________==___|>        - * -\n";
	message+= "    ===========|| /                                            |\n";
	message+= "        =======||/     /\\/\\   __ _ _ __   __ _  __ _  ___ _ __  \n";
	message+= "                      /    \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|\n";
	message+= "          |          / /\\/\\ \\ (_| | | | | (_| | (_| |  __/ |  \n";
	message+= "        - * -        \\/    \\/\\__,_|_| |_|\\__,_|\\__, |\\___|_| \n";
	message+= "          |                   _  _              |___/       \n";
	message+= "                  wow          )/ \\/||_|_ \n";
	message+= "                              /_\\_/ |  |              Gluten-free!\n";
	message+= "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
	message+= "Welcome to The Pro Quidditch Manager 2014!\n";
	return message;
}

string Message::goodBye(){
	string message;
	message+= "                 =========================            \n";
	message+= "   Thank you for playing the Pro Quidditch Manager 2014!\n";
	message+= "                   See you next time! :)\n";
	message+= "                 =========================            \n";
	return message;
}
