#include "GraphicMatchManager.hpp"
#include <toolbox.hpp>

GraphicMatchManager::GraphicMatchManager(ClientManager const & parent, GUI::MainController & controller) :
	MatchManager(parent),
	GUI::GraphicManager(controller), 
	_match(pitch()), 
	_selectedPlayer(NULL)
{}

void GraphicMatchManager::redraw()
{
	window().clear(sf::Color::White);
	window().draw(_match);
	window().display();
}

bool GraphicMatchManager::treatEvent(sf::Event const & ev)
{
	if (ev.type == sf::Event::Closed)
		return false;
	else if (ev.type == sf::Event::KeyPressed){
		switch (ev.key.code){
			case sf::Keyboard::Escape: return false;
			case sf::Keyboard::Space: redraw(); break;
			default: break;
		}
	} else if (ev.type == sf::Event::MouseButtonPressed){
		/* Click on pitch */
		Position GUI_click = Position(ev.mouseButton.x, ev.mouseButton.y);
		Position pos(_match.GUI2pitch(GUI_click));
		_match.clear();
		/* Hilight clicked pos */
		_match.hilight(pos, &UIMatch::hilightBlue);

		/* find if there is someone at this position */
		Moveable *atPos = pitch().getAt(pos);
		if (! _selectedPlayer && atPos && atPos->isPlayer()){
			cout << JSON::List(pos) << " <=> " << JSON::List(atPos->getPosition()) << endl;
			_selectedPlayer = (Player *) atPos;
			_match.hilightAccessibles(pos, _selectedPlayer->getSpeed());
		} 

		else if (atPos == _selectedPlayer) {
			/* Cancel move: click on player */
			_selectedPlayer = NULL;
			_currentMove = Displacement();
		} 

		else if (_selectedPlayer) {
			Position lastPos = _currentMove.position() + _selectedPlayer->getPosition();
			Position delta = pos - lastPos;
			size_t l = delta.length()+_currentMove.length();

			if (delta == Position(0, 0)){
				/* Click on last reached position: finish and send move */
				sendDisplacement(*_selectedPlayer, _currentMove);
				cout << "SENT MOVE " << JSON::List(_currentMove) << endl;
				_selectedPlayer = NULL;
				_currentMove = Displacement();
				_match.clear();
			}

			else if (delta.isDirection() && l <= _selectedPlayer->getSpeed()){
				size_t rest = _selectedPlayer->getSpeed() - l;
				_currentMove.addMove(delta);
				_match.clear(); /* clear hilights */
				
				_match.hilightDisplacement(_selectedPlayer->getPosition(), _currentMove);
				if (_currentMove.length() < _selectedPlayer->getSpeed()){
					_match.hilightAccessibles(pos, rest);
				}
			} else {
				cout << " &&&&& Reject move " << _selectedPlayer->getName() 
				     << " to " << JSON::List(pos) << " because delta "
				     << JSON::List(delta) << " is invalid" << endl;
			}
		}
		redraw();
	}
	return true;
}

void GraphicMatchManager::run()
{
	cout << "ENTER GraphicMatchManager::run" << endl;
	redraw();
	
	bool running = true;
	while (running && state() != FINISHED){
		sf::Event ev;
		bool hasReceived = false;

		if (window().pollEvent(ev)){
			if (! treatEvent(ev))
				running = false;
			hasReceived = true;
		}

		readMessages();

		if (! hasReceived)
			minisleep(0.01);
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
