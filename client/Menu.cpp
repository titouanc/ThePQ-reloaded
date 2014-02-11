#include "Menu.hpp"

using namespace std;

Menu::Menu()
{
	_message = "";
}

Menu::Menu(string message, char choice, void (*callback)())
{
	setMessage(message);
	addOption(choice, callback);
}

void Menu::setMessage(string message)
{
	_message = message;
}

void Menu::addOption(char choice, void (*callback)())
{
	if (choice != 'q' && choice != 'Q')
	{
		_options.insert(std::pair<char, void(*)()>(choice, callback));
	}
}

void Menu::run()
{
	char userChoice;
	do
	{
		cout << _message;
		cin >> userChoice;
		if (userChoice >= 'A' && userChoice <= 'Z')
		{
			userChoice -= 'A' - 'a';
		}
		map<char, void(*)()>::const_iterator it;
		it = _options.find(userChoice);
		if (it != _options.end())
		{
			it->second();
		}
	}
	while (userChoice != 'q' && userChoice != 'Q');
}
