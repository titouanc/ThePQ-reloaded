#ifndef MESSAGES_HPP
#define MESSAGES_HPP 

#include <cstddef>
#include <iostream>

class Message
{
public:
	static void printPrompt();
	static void printLaunchingMessage();
	static void printLoggedOffActions();
	static void printMainMenu();
	static void printManagementMenu();
	static void printGoodbyeMessage();
};


#endif
