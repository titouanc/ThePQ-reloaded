#ifndef _MENU_HPP
#define _MENU_HPP
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Menu {
	private:
		std::vector<string> _inBuffer;
		int _option;
		int size;
	public:
		Menu(){_option=0;size=0;};
		//Menu(string inBuffer): _inBuffer(inBuffer) {};
		int getOption(){return _option;}
		int getSize(){return _inBuffer.size(); }
		void clearBuffer();
		void showMenu();
		void addToDisplay(string c);
		int run();
		
	static std::string askForUserData(string prompt)
	{
		string data;
		cout << endl;
		cout << prompt;
		cin >> data;
		return data;
	}
};
#endif
