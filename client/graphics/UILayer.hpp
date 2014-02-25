#ifndef UILAYER_HPP
#define UILAYER_HPP 

#include <SFML/Graphics.hpp>
#include <list>
#include "UIClickable.hpp"

class UILayer {
	UILayer(sf::Color backgroundColor=sf::Color(0xff, 0xff, 0xff, 0xff)) 
			_active(false), _backgroundColor(backgroundColor){}
	bool isActive() 	{ return _active; }
	void activate() 	{ _active = true; }
	void deactivate() 	{ active = false; }

	void handleClick(int x, int y);
private:
	bool _active;
	sf::Color _backgroundColor;
	std::list<UIClickable&> _clickables;
};

#endif