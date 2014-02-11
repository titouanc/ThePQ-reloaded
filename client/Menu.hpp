#ifndef __MENU_HPP
#define __MENU_HPP

#include <map>
#include <iostream>
#include "Message.hpp"

class Menu
{
public:
	Menu();
	Menu(std::string message, char choice, void (*callback)());
	void setMessage(std::string message);
	void addOption(char choice, void (*callback)());
	void run();
	
	
private:
	std::string _message;
	std::map<char, void(*)()> _options;
	
};

#endif // __MENU_HPP
