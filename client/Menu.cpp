#include "Menu.hpp"

using namespace std;

Menu::Menu()
{
	_message = "";
}

Menu::Menu(string message, std::string choice, void (*callback)())
{
	setMessage(message);
	addOption(choice, callback);
}

void Menu::setMessage(string message)
{
	_message = message;
}

void Menu::addOption(std::string choice, void (*callback)())
{
	if (choice != "q" && choice != "Q")
	{
		_options.insert(std::pair<std::string, void(*)()>(choice, callback));
	}
}

void Menu::run()
{
	string userChoice;
	do
	{
		cout << _message;
		cin >> userChoice;
		if (userChoice[0] >= 'A' && userChoice[0] <= 'Z')
		{
			userChoice[0] -= 'A' - 'a';
		}
		map<std::string, void(*)()>::const_iterator it;
		it = _options.find(userChoice);
		if (it != _options.end())
		{
			it->second();
		}
	}
	while (userChoice != "q" && userChoice != "Q");
}
