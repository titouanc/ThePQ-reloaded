#include "Message.hpp"

using namespace std;

string Message::printPrompt(){
	string ret;
	ret+= "> ";
	return ret;
}

string Message::printLaunchingMessage(){
	string ret;
	ret+= "           __       _   \n";
	ret+= "           //_ _   /_/_ _                  much amazing  |\n";
	ret+= "          // //_' /  //_/                              - * -\n";
	ret+= "      ____       _     _     _     _ _ _       _         |\n";
	ret+= "     /___ \\_   _(_) __| | __| | __| (_) |_ ___| |__  \n";
	ret+= "    //  / / | | | |/ _` |/ _` |/ _` | | __/ __| '_ \\ \n";
	ret+= "   / \\_/ /| |_| | | (_| | (_| | (_| | | || (__| | | |\n";
	ret+= "   \\___,_\\ \\__,_|_|\\__,_|\\__,_|\\__,_|_|\\__\\___|_| |_|\n";
	ret+= "        =======||\\\n";
	ret+= "    ===========|| \\___________________________  ___            |\n";
	ret+= "  =============||  |__________________________==___|>        - * -\n";
	ret+= "    ===========|| /                                            |\n";
	ret+= "        =======||/     /\\/\\   __ _ _ __   __ _  __ _  ___ _ __  \n";
	ret+= "                      /    \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|\n";
	ret+= "          |          / /\\/\\ \\ (_| | | | | (_| | (_| |  __/ |  \n";
	ret+= "        - * -        \\/    \\/\\__,_|_| |_|\\__,_|\\__, |\\___|_| \n";
	ret+= "          |                   _  _              |___/       \n";
	ret+= "                  wow          )/ \\/||_|_ \n";
	ret+= "                              /_\\_/ |  |              Gluten-free!\n";
	ret+= "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
	ret+= "Welcome to The Pro Quidditch Manager 2014!\n";
	return ret;
}

string Message::printLoggedOffActions(){
	string ret;
	ret+= "You can : \n";
	ret+= "   - (l)ogin\n";
	ret+= "   - (r)egister\n";
	ret+= "   - (q)uit\n";
	return ret;
}

string Message::printMainMenu(){
	string ret;
	ret+= "You can : \n";
	ret+= "   - (m)anage your team and stadium\n";
	ret+= "   - (p)lay a friendly game\n";
	ret+= "   - (q)uit\n";
	ret+= printPrompt();
	return ret;
}

string Message::printManagementMenu(){
	string ret;
	ret+= "You can : \n";
	ret+= "   - manage your (s)tadium and installations\n";
	ret+= "   - manage your (p)layers\n";
	ret+= "   - (q)uit to main menu\n";
	ret+= printPrompt();
	return ret;
}

string Message::printGoodbyeMessage(){
	string ret;
	ret+= "                 =========================            \n";
	ret+= "   Thank you for playing the Pro Quidditch Manager 2014!\n";
	ret+= "                   See you next time! :)\n";
	ret+= "                 =========================            \n";
	ret+= printPrompt();
	return ret;
}
