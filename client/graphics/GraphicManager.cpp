#include "GraphicManager.hpp"


void GUI::GraphicManager::run(){
	_controller.window.display();
	while(_controller.window.isOpen() && _isRunning){
		sf::Event event;
		_controller.window.waitEvent(event);
		if (event.type == sf::Event::MouseButtonPressed 
			&& event.mouseButton.button == sf::Mouse::Left){
			_controller.handleClick(event);
		}
		else if (event.type == sf::Event::Closed ||
				(event.type==sf::Event::KeyPressed 
				&& event.key.code==sf::Keyboard::Escape)){
			_controller.window.close();
		}
	}
}

void GUI::GraphicManager::deleteCanvas() {
	_isRunning = false;
	_controller.deleteTopLayer();
}

void GUI::GraphicManager::displayCanvas() {
	_controller.addLayer(_canvas);
}