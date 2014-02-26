#ifndef UICLICKABLE_HPP
#define UICLICKABLE_HPP 

#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class UIClickableInterface {
public:
	virtual void triggerAction() = 0;
	virtual bool isInBounds(int x, int y) const = 0;
	virtual ~UIClickableInterface() {};
	virtual void renderTo(sf::RenderTarget& dest) = 0;
};

template <typename T> class UIClickable : public UIClickableInterface {
public:
	typedef std::function<void(T*)> Callback;
	UIClickable(const Callback& callback, T* target) : _callback(callback), _target(target) {}
	~UIClickable(){}
	void triggerAction(){ _callback(_target); }
	virtual bool isInBounds(int x, int y) const = 0;
	virtual void renderTo(sf::RenderTarget& dest) = 0;
private:
	Callback _callback;
	T* _target;
};

#endif