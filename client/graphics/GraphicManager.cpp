#include "GraphicManager.hpp"

void GUI::GraphicManager::run()
{
	while(_controller.window.isOpen() && _isRunning){
		sf::Event event;
		if (_controller.window.waitEvent(event))
			treatEvent(event);
	}
}

bool GUI::GraphicManager::readEvent()
{
	sf::Event event;
	if (! _controller.window.pollEvent(event))
		return false;
	treatEvent(event);
	return true;
}

bool GUI::GraphicManager::treatEvent(sf::Event const & event)
{
	if (event.type == sf::Event::GainedFocus){
		redrawCanvas();
	}
	else if (event.type == sf::Event::MouseButtonPressed){
		if (event.mouseButton.button == sf::Mouse::Left)
			_controller.handleClick(event);
		else if (event.mouseButton.button == sf::Mouse::Right)
			_controller.handleRightClick(event);
	}
	else if (event.type == sf::Event::TextEntered){
		_controller.handleTextEntered(event);
	}
	else if (event.type == sf::Event::Closed ||
			(event.type==sf::Event::KeyPressed 
			&& event.key.code==sf::Keyboard::Escape)){
		window().close();
	}
	return true;
}

void GUI::GraphicManager::deleteCanvas()
{
	//_isRunning = false;
	_controller.deleteTopLayer();
}

void GUI::GraphicManager::displayCanvas()
{
	_controller.addLayer(_canvas);
}

void GUI::GraphicManager::redrawCanvas()
{
	_canvas.renderTo(_controller.window);
	window().display();
}

GUI::Button<GUI::GraphicManager> & GUI::GraphicManager::backButton(std::string const & caption)
{
	return _canvas.addButton<GUI::GraphicManager>(
		&GUI::GraphicManager::stop, this, caption
	);
}
