#ifndef MESSAGE_HPP
#define MESSAGE_HPP 

#include <string>

class Message
{
public:
	static std::string splashScreen();
	static std::string loginMenu();
	static std::string mainMenu();
	static std::string managmentMenu();
	static std::string goodBye();
	
private:
	static std::string prompt();
};


#endif
