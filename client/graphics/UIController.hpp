#ifndef UICONTROLLER_HPP
#define UICONTROLLER_HPP

#include <SFML/Window.hpp>
#include <stack>
#include "UILayer.hpp"

class UIController {
public:
	UIController();
	void addNewLayer(UILayer& layer);
	void deleteTopLayer();

	void handleClick(sf::Event e);

private:
	sf::RenderWindow _window;
	std::stack<UILayer&> _layers;
};

#endif