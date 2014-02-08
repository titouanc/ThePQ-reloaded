#include <iostream>
#include "Messages.hpp"

using namespace std;

void printPrompt(){
	cout << "> ";
}

void printLaunchingMessage(){
	cout << "           __       _   " << endl;
	cout << "           //_ _   /_/_ _                  much amazing  |" << endl;
	cout << "          // //_' /  //_/                              - * -" << endl;
	cout << "      ____       _     _     _     _ _ _       _         |" << endl;
	cout << "     /___ \\_   _(_) __| | __| | __| (_) |_ ___| |__  " << endl;
	cout << "    //  / / | | | |/ _` |/ _` |/ _` | | __/ __| '_ \\ " << endl;
	cout << "   / \\_/ /| |_| | | (_| | (_| | (_| | | || (__| | | |" << endl;
	cout << "   \\___,_\\ \\__,_|_|\\__,_|\\__,_|\\__,_|_|\\__\\___|_| |_|" << endl;
	cout << "        =======||\\" << endl;
	cout << "    ===========|| \\___________________________  ___            |" << endl;
	cout << "  =============||  |__________________________==___|>        - * -" << endl;
	cout << "    ===========|| /                                            |" << endl;
	cout << "        =======||/     /\\/\\   __ _ _ __   __ _  __ _  ___ _ __  " << endl;
	cout << "                      /    \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|" << endl;
	cout << "          |          / /\\/\\ \\ (_| | | | | (_| | (_| |  __/ |  " << endl;
	cout << "        - * -        \\/    \\/\\__,_|_| |_|\\__,_|\\__, |\\___|_| " << endl;
	cout << "          |                   _  _              |___/       " << endl;
	cout << "                  wow          )/ \\/||_|_ " << endl;
	cout << "                              /_\\_/ |  |              Gluten-free!" << endl;
	cout << endl;
	cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
	cout << endl;
	cout << "Welcome to The Pro Quidditch Manager 2014!" << endl;
}

void printLoggedOffActions(){
	cout << "You can : "<< endl;
	cout << "   - (l)ogin" << endl;
	cout << "   - (r)egister" << endl;
	cout << "   - (q)uit" << endl;
	printPrompt();
}

void printMainMenu(){
	cout << "You can : "<< endl;
	cout << "   - (m)anage your team and stadium" << endl;
	cout << "   - (p)lay a friendly game" << endl;
	cout << "   - (q)uit" << endl;
	printPrompt();
}

void printManagementMenu(){
	cout << "You can : "<< endl;
	cout << "   - manage your (s)tadium and installations" << endl;
	cout << "   - manage your (p)layers" << endl;
	cout << "   - (q)uit to main menu" << endl;
	printPrompt();
}

void printGoodbyeMessage(){
	cout << endl;
	cout << "                 =========================            " << endl;
	cout << "   Thank you for playing the Pro Quidditch Manager 2014!" << endl;
	cout << "                   See you next time! :)" << endl;
	cout << "                 =========================            " << endl << endl;
}