#ifndef MESSAGE_HPP
#define MESSAGE_HPP 

#include <string>

class Message
{
public:
	static std::string printPrompt();
	static std::string printLaunchingMessage();
	static std::string printLoggedOffActions();
	static std::string printMainMenu();
	static std::string printManagementMenu();
	static std::string printGoodbyeMessage();
};


#endif
