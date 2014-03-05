#include <GraphicMatchManager.hpp>

void GraphicMatchManager::run()
{
	cout << "ENTER GraphicMatchManager::run" << endl;
	window().draw(_match);
	window().display();
	
	bool running = true;
	while (running){
		cout << " + tick GraphicMatchManager::run" << endl;
		sf::Event ev;
		window().waitEvent(ev);
		if (ev.type == sf::Event::Closed)
			window().close();
		if (ev.type == sf::Event::KeyPressed)
			running = false;
	}

	cout << "LEAVE GraphicMatchManager::run" << endl;
}