#include "MainController.hpp"

using namespace GUI;

GUI::MainController::MainController(): window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "The Pro Quidditch"){
	window.setFramerateLimit(60);
	window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));
}

void GUI::MainController::addLayer(Layer & layer){
	if (_layers.size() != 0)
		_layers.top()->deactivate();
	_layers.push(&layer);
	layer.renderTo(window);
	layer.activate();
}

void GUI::MainController::deleteTopLayer(){
	if (_layers.size() != 0){
		_layers.pop();
		// don't delete the layer that has just been popped, it will be deleted by
		// its manager
		if (_layers.size() != 0){
			_layers.top()->activate();
			_layers.top()->renderTo(window);
		}
	}
	window.display();
}

void GUI::MainController::handleClick(sf::Event e){
	if (_layers.size() != 0)
		_layers.top()->handleClick(e.mouseButton.x, e.mouseButton.y);
}
void GUI::MainController::handleRightClick(sf::Event e){
	if (_layers.size() != 0){
		_layers.top()->handleRightClick(e.mouseButton.x, e.mouseButton.y);
		_layers.top()->renderTo(window);
	}
	window.display();
}

void GUI::MainController::handleTextEntered(sf::Event e){
	if (_layers.size() != 0){
		_layers.top()->handleTextEntered(e);
		_layers.top()->renderTo(window);
	}
	window.display();
}