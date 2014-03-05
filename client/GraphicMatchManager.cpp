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
				running = false;
			else if (ev.type == sf::Event::KeyPressed){
				switch (ev.key.code){
					case sf::Keyboard::Escape: running = false; break;
					case sf::Keyboard::Space: redraw(); break;
					default: break;
				}
			} else if (ev.type == sf::Event::MouseButtonPressed){
				Position GUI_click = Position(ev.mouseButton.x, ev.mouseButton.y);
				Position pos(_match.GUI2pitch(GUI_click));
				_match.clear();
				_match.hilight(pos, &UIMatch::hilightBlue);
				redraw();
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
