#ifndef UILAYER_HPP
#define UILAYER_HPP 

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UIButton.hpp"
#include "GUIConstants.hpp"

class UILayer {
public:
	UILayer(sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff)): _active(false), _backgroundColor(backgroundColor){}
	~UILayer();
	bool isActive() 	{ return _active; }
	void activate() 	{ _active = true; }
	void deactivate() 	{ _active = false; }

	void handleClick(int x, int y);

	void renderTo(sf::RenderTarget & dest);

	template <typename T> UIButton<T>* addButton(const typename UIClickable<T>::Callback& callback, 
												T* target, std::string text="Button");

private:
	bool _active;
	sf::Color _backgroundColor;
	std::vector<UIClickableInterface*> _clickables;
};

template <typename T>
UIButton<T> * UILayer::addButton(const typename UIClickable<T>::Callback& callback, 
								T* target, string text) {
	UIButton<T> * toAdd = new UIButton<T>(callback, target, text);
	_clickables.push_back((UIClickableInterface*)toAdd);
	return toAdd;
}

#endif