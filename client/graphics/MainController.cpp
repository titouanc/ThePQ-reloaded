#include "MainController.hpp"

using namespace GUI;

GUI::MainController::MainController(): window(	sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
												"The Pro Quidditch", 
												sf::Style::Titlebar|sf::Style::Close)
{
	window.setFramerateLimit(60);
	window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));
	
	if (mainMusic.openFromFile(soundPath("futura.ogg")))
	{
		mainMusic.play();
	}
}

GUI::MainController::~MainController()
{

}

void GUI::MainController::addLayer(Layer & layer)
{
	if (_layers.size() != 0){
		if (layer.getBackgroundColor().a != 0xff)
			_layers.top()->renderTo(window);
	}
	_layers.push(&layer);
	layer.renderTo(window);
}

void GUI::MainController::deleteTopLayer()
{
	if (_layers.size() != 0){
		_layers.pop();
		// don't delete the layer that has just been popped, it will be deleted by
		// its manager
		if (_layers.size() != 0){
			_layers.top()->renderTo(window);
		}
	}
	window.display();
}

void GUI::MainController::renderTopLayer()
{
	// this methods allows for one (only one) semi-transparent layer on top of another one
	if (_layers.size() != 0){
		window.clear();
		Layer * topLayer = _layers.top();
		_layers.pop();
		if (_layers.size() != 0 && topLayer->getBackgroundColor().a != 0xff)
			_layers.top()->renderTo(window);
		_layers.push(topLayer);
		_layers.top()->renderTo(window);
		window.display();
	}
}

bool GUI::MainController::handleClick(sf::Event e)
{
	bool res = false;
	if (_layers.size() != 0)
		res = _layers.top()->handleClick(e.mouseButton.x, e.mouseButton.y);
	renderTopLayer();
	return res;
}
bool GUI::MainController::handleRightClick(sf::Event e)
{
	bool res = false;
	if (_layers.size() != 0)
		res = _layers.top()->handleRightClick(e.mouseButton.x, e.mouseButton.y);
	renderTopLayer();
	return res;
}

void GUI::MainController::handleTextEntered(sf::Event e)
{
	if (_layers.size() != 0){
		_layers.top()->handleTextEntered(e);
		_layers.top()->renderTo(window);
	}
	window.display();
}

void GUI::MainController::handleMouseMoved(sf::Event e)
{
	if (_layers.size() != 0)
		_layers.top()->handleMouseMoved(e);
	renderTopLayer();
}
