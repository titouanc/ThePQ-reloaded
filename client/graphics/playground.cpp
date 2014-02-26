#include <SFML/Window.hpp>
#include <iostream>
#include "UIButton.hpp"
#include "MainController.hpp"
#include <string>

using namespace std;
using namespace GUI;

class GraphicManager {
public:
	GraphicManager(MainController &uic) : _uic(uic), _isRunning(true){}
	void run(){
		_uic.window.display();
		while(_uic.window.isOpen() && _isRunning){
			sf::Event event;
			_uic.window.waitEvent(event);
			if (event.type == sf::Event::MouseButtonPressed 
				&& event.mouseButton.button == sf::Mouse::Left){
				_uic.handleClick(event);
			}
			else if (event.type == sf::Event::Closed ||
					(event.type==sf::Event::KeyPressed 
					&& event.key.code==sf::Keyboard::Escape)){
				_uic.window.close();
			}
		}
	}
	void disappear() {
		_isRunning = false;
		_uic.deleteTopLayer();
	}
protected:
	MainController &_uic;
	UILayer _layer;
	bool _isRunning;
};

class GraphicManager2 : public GraphicManager {
public:
	GraphicManager2(MainController &uic) : GraphicManager(uic){
		UIButton<GraphicManager2> *button = _layer.addButton<GraphicManager2>(&GraphicManager2::superMethod, this, "gm 2");
		button->setPos(100, 100);
		_uic.addLayer(_layer);
		run();
	}
	~GraphicManager2(){}

	void superMethod() { 
		cout << "deleting gm2" << endl; 
		disappear();
	}

};

class GraphicManager1 : public GraphicManager {
public:
	GraphicManager1(MainController &uic) : GraphicManager(uic){
		_layer.addButton<GraphicManager1>(&GraphicManager1::myMethod, this, "gm 1");
		_uic.addLayer(_layer);
		run();
	}

	void myMethod() { 
		cout << "constructing gm2!" << endl; 
		GraphicManager2 gm2(_uic);
	}
};



int main(int argc, char const *argv[])
{
	//sf::RenderWindow window(sf::VideoMode(1280, 720), "The Pro Quidditch");
	//window.setFramerateLimit(60);

	MainController controller;
	GraphicManager1 d(controller);
	//controller.run();
	//GraphicManager2 od(&controller);
	/*
	try{
		newButton.renderTo(window);
		otherButton.renderTo(window);
	} catch (const char *msg){
		cerr << msg << endl;
		return EXIT_FAILURE;
	}
	*/
	/*
	window.display();
	while(window.isOpen()){
		sf::Event ev;
		window.waitEvent(ev);
		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left){
			if (newButton.isInBounds(ev.mouseButton.x, ev.mouseButton.y)) {
				newButton.triggerAction();
			}
			if (otherButton.isInBounds(ev.mouseButton.x, ev.mouseButton.y)) {
				otherButton.triggerAction();
			}
		}
		if (
			(ev.type == sf::Event::Closed) ||
			(ev.type==sf::Event::KeyPressed && ev.key.code==sf::Keyboard::Escape)
		)
			window.close();
	}
	*/
	return 0;
}

