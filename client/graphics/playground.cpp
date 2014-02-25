#include <SFML/Window.hpp>
#include <iostream>
#include "Button.hpp"

using namespace std;

class Dummy {
public:
	void myMethod() { cout << "called!" << endl; }
};

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "The Pro Quidditch");
	window.setFramerateLimit(60);
	window.clear(sf::Color(0xff, 0xff, 0xff, 0xff));

	Dummy d;
	Button<Dummy> newButton(&Dummy::myMethod, &d);

	try{
		newButton.renderTo(window);
	} catch (const char *msg){
		cerr << msg << endl;
		return EXIT_FAILURE;
	}

	window.display();
	while(window.isOpen()){
		sf::Event ev;
		window.waitEvent(ev);
		if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left){
			if (ev.mouseButton.x <= 110 && ev.mouseButton.x >=10 && ev.mouseButton.y >=10 && ev.mouseButton.y <=50){
				newButton.triggerAction();
			}
		}
	}
	return 0;
}

