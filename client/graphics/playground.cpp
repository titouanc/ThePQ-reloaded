#include <iostream>
#include <string>

#include "GraphicManager.hpp"

using namespace std;
using namespace GUI;

class GraphicManager2 : public GraphicManager {
public:
	GraphicManager2(MainController &uic) : GraphicManager(uic){
		Button<GraphicManager2> *button = _canvas.addButton<GraphicManager2>(&GraphicManager2::superMethod, this, "gm 2");
		button->setPos(100, 100);
		displayCanvas();
		run();
	}
	~GraphicManager2(){}

	void superMethod() { 
		cout << "deleting gm2" << endl; 
		deleteCanvas();
	}

};

class GraphicManager1 : public GraphicManager {
public:
	GraphicManager1(MainController &uic) : GraphicManager(uic){
		_canvas.addButton<GraphicManager1>(&GraphicManager1::myMethod, this, "gm 1");
		displayCanvas();
		run();
	}

	void myMethod() { 
		cout << "constructing gm2!" << endl; 
		GraphicManager2 gm2(_controller);
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

