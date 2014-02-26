#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include <SFML/Window.hpp>
#include <stack>
#include "UILayer.hpp"
#include "GUIConstants.hpp"

class UIController {
public:
	UIController();
	void addLayer(UILayer& layer);
	void deleteTopLayer();

	void handleClick(sf::Event e);
	sf::RenderWindow _window;


private:
	
	std::stack<UILayer*> _layers;
};

#endif