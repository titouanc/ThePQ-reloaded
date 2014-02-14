#ifndef __MENU_HPP
#define __MENU_HPP

#include <map>
#include <iostream>
#include "Message.hpp"

struct Callback {
  virtual ~Callback(){}
  virtual void operator()()=0;
};

template<class T>
class ClassCallback : public Callback {
  T* _classPtr;
  typedef void(T::*fncb)();
  fncb _cbProc;
public:
  ClassCallback(T* classPtr,fncb cbProc):_classPtr(classPtr),_cbProc(cbProc){}
  virtual void operator()(){
    (_classPtr->*_cbProc)();
  }
};

class Menu
{
public:
	Menu();
	void setMessage(std::string message);
	void addOption(char choice, Callback* callback);
	void run();
	
	
private:
	std::string _message;
	std::map<char, Callback*> _options;
	
};

#endif // __MENU_HPP
