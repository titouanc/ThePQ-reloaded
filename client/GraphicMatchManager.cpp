#include "GraphicMatchManager.hpp"

void GraphicMatchManager::redraw()
{
	window().clear(sf::Color::White);
	window().draw(_match);
	window().display();
}

void GraphicMatchManager::run()
{
	cout << "ENTER GraphicMatchManager::run" << endl;
	redraw();
	
	bool running = true;
	while (running){
		sf::Event ev;
		if (window().pollEvent(ev)){
			if (ev.type == sf::Event::Closed)
				window().close();
			if (ev.type == sf::Event::KeyPressed){
				switch (ev.key.code){
					case sf::Keyboard::Escape: running = false; break;
					case sf::Keyboard::Space: redraw(); break;
					default: break;
				}
			}
		}

		if (connection().hasMessage()){
			JSON::Value *msg = connection().popMessage();
			treatMessage(DICT(msg));
			delete msg;
		}
	}

	cout << "LEAVE GraphicMatchManager::run" << endl;
}

void GraphicMatchManager::onPitchChange()
{
	redraw();
}

void GraphicMatchManager::onStateChange()
{
	cout << "CHANGE STATE: ";
	switch (state()){
		case CREATED:  cout << "created" << endl; break;
		case PROMPT:   cout << "waiting for moves" << endl; break;
		case TIMEOUT:  cout << "processing moves" << endl; break;
		case FINISHED: cout << "finished" << endl; break;
	}
}
