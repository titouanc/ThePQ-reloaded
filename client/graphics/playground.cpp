#include <SFML/Window.hpp>
#include <iostream>
#include "UIButton.hpp"
#include "UIController.hpp"

using namespace std;

class GraphicManager2 {
public:
	GraphicManager2(UIController &uic) : _uic(uic), _isRunning(true){
		UIButton<GraphicManager2> *button = _layer.addButton<GraphicManager2>(&GraphicManager2::superMethod, this, "gm 2");
		button->setPos(100, 100);
		_uic.addLayer(_layer);

	}
	~GraphicManager2(){
	}
	void run(){
		_uic.run();
	}
	void superMethod() { 
		cout << "deleting gm2" << endl; 
		_uic.deleteTopLayer();
	}
private:
	UIController &_uic;
	UILayer _layer;
	bool _isRunning;
};

class GraphicManager1 {
public:
	GraphicManager1(UIController &uic) : _uic(uic){
		_layer.addButton<GraphicManager1>(&GraphicManager1::myMethod, this, "gm 1");
		_uic.addLayer(_layer);
	}

	void myMethod() { 
		cout << "constructing gm2!" << endl; 
		GraphicManager2 gm2(_uic);
		gm2.run();
	}
private:
	UIController &_uic;
	UILayer _layer;
	bool _isRunning;
};



int main(int argc, char const *argv[])
{
	//sf::RenderWindow window(sf::VideoMode(1280, 720), "The Pro Quidditch");
	//window.setFramerateLimit(60);

	UIController controller;
	cout << "a" << endl;	
	GraphicManager1 d(controller);
	cout << "b" << endl;
	controller.run();
	cout << "c" << endl;
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

