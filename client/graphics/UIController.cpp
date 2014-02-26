#include "UIController.hpp"

UIController::UIController(): _window(sf::VideoMode(GUI::WINDOW_WIDTH, GUI::WINDOW_HEIGHT), "The Pro Quidditch"){
	_window.setFramerateLimit(60);
	_window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));
}

void UIController::addLayer(UILayer & layer){
	if (_layers.size() != 0)
		_layers.top()->deactivate();
	_layers.push(&layer);
	layer.renderTo(_window);
	layer.activate();
}

void UIController::deleteTopLayer(){
	if (_layers.size() != 0){
		_layers.pop();
		if (_layers.size() != 0){
			_layers.top()->activate();
			_layers.top()->renderTo(_window);
		}
	}
}

void UIController::handleClick(sf::Event e){
	if (_layers.size() != 0)
		_layers.top()->handleClick(e.mouseButton.x, e.mouseButton.y);
}