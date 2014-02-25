#include "UIController.hpp"

UIController::UIController(): _window(sf::VideoMode(1280, 720)){
	_window.setFramerateLimit(60);
	_window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));

	_window.display();
	while(_window.isOpen()){
		sf::Event event;
		_window.waitEvent(event);
		if (event == sf::Event::MouseButtonPressed 
			&& event.mouseButton.button == sf::Mouse::Left){
			handleClick(event);
		}
		else if (ev.type == sf::Event::Closed) ||
				(ev.type==sf::Event::KeyPressed 
				&& ev.key.code==sf::Keyboard::Escape){
			_window.close();
		}
	}
}

void UIController::addNewLayer(UILayer & layer){
	_layers.top().deactivate();
	_layers.push(layer);
	layer.activate();
}

void UIController::deleteTopLayer(){
	_layers.pop();
	_layers.top().activate();
}

void UIController::handleClick(sf::Event e){
	_layers.top().handleClick(e.mouseButton.x, e.mouseButton.y);
}