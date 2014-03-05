#ifndef __NOTIFICATION_MANAGER_HPP
#define __NOTIFICATION_MANAGER_HPP

#include "ClientManager.hpp"
#include <json/json.hpp>
#include <map>

class AbstractCallback {
public:
	virtual void operator()(JSON::Value const *) = 0;
	virtual ~AbstractCallback() {};
};

template<class T>
class ClassCallback : public AbstractCallback {
  T* _classPtr;
  typedef void(T::*fncb)(JSON::Value const * json);
  fncb _cbProc;
public:
  ClassCallback(T* classPtr,fncb cbProc):_classPtr(classPtr),_cbProc(cbProc){}
  virtual void operator()(JSON::Value const * json){
    (_classPtr->*_cbProc)(json);
  }
};

class NotificationManager : public ClientManager
{
public:
	using ClientManager::ClientManager;
	void addCallback(pair<string, AbstractCallback*>);
	void handleNotification(JSON::Value *notification);
	void handleAllNotifications();
	void loadTeam(JSON::Value const * funds);

private:
	std::map<std::string, AbstractCallback*> _callbacks;
};

#endif // __NOTIFICATION_MANAGER_HPP
