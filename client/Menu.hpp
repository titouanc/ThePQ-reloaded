#ifndef __MENU_HPP
#define __MENU_HPP

#include <map>
#include <iostream>
#include "Message.hpp"

class Menu
{
public:
	Menu();
	Menu(std::string message, std::string choice, void (*callback)(void));
	void setMessage(std::string message);
	void addOption(std::string choice, void (*callback)(void));
	void run();
	
	
private:
	std::string _message;
	std::map<std::string, void(*)(void)> _options;
	
};

#endif // __MENU_HPP
