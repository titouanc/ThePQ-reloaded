#ifndef UICLICKABLE_HPP
#define UICLICKABLE_HPP 

#include <functional>

template <typename T> class UIClickable {
public:
	typedef std::function<void(T*)> Callback;
	UIClickable(const Callback& callback, T* target) : _callback(callback), _target(target) {}
	void triggerAction(){ _callback(_target); }
	virtual bool isInBounds(int x, int y) const = 0;
private:
	Callback _callback;
	T* _target;
};

#endif