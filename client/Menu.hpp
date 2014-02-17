#ifndef __MENU_HPP
#define __MENU_HPP

#include <map>
#include <iostream>
#include "Message.hpp"


template<class T>
class ClassCallback {
  T* _classPtr;
  typedef void(T::*fncb)();
  fncb _cbProc;
public:
  ClassCallback(T* classPtr,fncb cbProc):_classPtr(classPtr),_cbProc(cbProc){}
  virtual void operator()(){
    (_classPtr->*_cbProc)();
  }
  virtual ~ClassCallback() {}
};

template<typename T>
class Menu
{
public:
	Menu();
	~Menu();
	void setMessage(std::string message);
	void addOption(char choice, ClassCallback<T>* callback);
	void run();
	
	
private:
	std::string _message;
	std::map<char, ClassCallback<T>*> _options;
	
};


template<typename T>
Menu<T>::Menu()
{
	_message = "";
}

template<typename T>
Menu<T>::~Menu()
{
	typename
	std::map<char, ClassCallback<T>*>::iterator it = _options.begin();
	while (it != _options.end())
	{
		delete it->second;
		it++;
	}
}

template<typename T>
void Menu<T>::setMessage(std::string message)
{
	_message = message;
}

template<typename T>
void Menu<T>::addOption(char choice, ClassCallback<T>* callback)
{
	if (choice != 'q' && choice != 'Q')
	{
		_options.insert(std::pair<char, ClassCallback<T>*>(choice, callback));
	}
}

template<typename T>
void Menu<T>::run()
{
	char userChoice;
	do
	{
		std::cout << _message;
		std::cin >> userChoice;
		if (userChoice >= 'A' && userChoice <= 'Z')
		{
			userChoice -= 'A' - 'a';
		}
		typename
		std::map<char, ClassCallback<T>*>::const_iterator it;
		it = _options.find(userChoice);
		if (it != _options.end())
		{
			(*(it->second))();
		}
	}
	while (userChoice != 'q' && userChoice != 'Q');
}

#endif // __MENU_HPP
