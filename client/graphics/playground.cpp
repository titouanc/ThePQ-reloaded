#include <SFML/Window.hpp>
#include <iostream>
#include "Button.hpp"

using namespace std;

class Dummy {
public:
	void myMethod() { cout << "called!" << endl; }
};

class OtherDummy {
public:
	void superMethod() { cout << "other dummy!" << endl; }
};

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "The Pro Quidditch");
	window.setFramerateLimit(60);
	window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));

	Dummy d;
	Button<Dummy> newButton(&Dummy::myMethod, &d, "This button's size is determined by its text.");

	OtherDummy od;
	Button<OtherDummy> otherButton(&OtherDummy::superMethod, &od, "Wow", 10, 60);
	try{
		newButton.renderTo(window);
		otherButton.renderTo(window);
	} catch (const char *msg){
		cerr << msg << endl;
		return EXIT_FAILURE;
	}

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
	return 0;
}

