#include "GraphicMatchView.hpp"
#include <toolbox.hpp>

///Constructor
GraphicMatchView::GraphicMatchView(ClientController const & parent, GUI::MainController & controller) :
	MatchController(parent),
	GUI::GraphicManager(controller), 
	_match(pitch(), mySquad()), 
	_selectedPlayer(NULL), _throwBall(false), _tooltipped(NULL)
{
	controller.mainMusic.stop();
	if (controller.mainMusic.openFromFile(soundPath("MarvelousSpaceAdventures.ogg"))){
		controller.mainMusic.setLoop(true);
		controller.mainMusic.play();
	}
}

GraphicMatchView::~GraphicMatchView()
{
	_controller.mainMusic.stop();
	if (_controller.mainMusic.openFromFile(soundPath("futura.ogg"))){
		_controller.mainMusic.setLoop(true);
		_controller.mainMusic.play();
	}
}

/**
 *
 */
void GraphicMatchView::redraw()
{
	window().clear(sf::Color::White);
	window().draw(_match);
	window().display();
}

/**
 * Method
 */
void GraphicMatchView::treatClick(Position const & pos, bool regularClick)
{
	_match.clear();
	/* Hilight clicked pos */
	_match.hilight(pos, &UIMatch::hilightBlue);

	/* find if there is someone at this position */
	Moveable *atPos = pitch().getAt(pos);
	if (! _selectedPlayer && atPos && atPos->isPlayer() && 
		isMyPlayer(*((Player*)atPos))){
		_selectedPlayer = (Player *) atPos;
		_match.hilightAccessibles(pos, _selectedPlayer->getSpeed());
	} 

	else if (atPos == _selectedPlayer && regularClick) {
		/* Cancel move: click on player */
		_selectedPlayer = NULL;
		_currentMove = Displacement();
		_throwBall = false;
	} 

	else if (_selectedPlayer) {
		Position lastPos = _currentMove.position() + _selectedPlayer->getPosition();
		Position delta = pos - lastPos;
		size_t l = delta.length()+_currentMove.length();

		if (delta == Position(0, 0)){
			if (regularClick){
				/* Left-Click on last reached position: finish and send move */
				sendDisplacement(*_selectedPlayer, _currentMove);
				cout << "SENT MOVE " << JSON::List(_currentMove) << endl;
				_selectedPlayer = NULL;
				_currentMove = Displacement();
				_match.clear();
			} else {
				/* Right-Click on last reached position: maybe throw quaffle */
				if (_selectedPlayer->canQuaffle()){
					PlayerQuaffle & pq = (PlayerQuaffle &) *_selectedPlayer;
					if (pq.hasQuaffle()){
						_throwBall = true;
						_match.hilightAccessibles(pos, 5, &UIMatch::hilightBlue);
					}
				}
			}
		}

		else if (_throwBall && delta.isDirection()){
			sendStroke(Stroke(
				*_selectedPlayer, _currentMove, ACT_THROW, lastPos, delta));
			_selectedPlayer = NULL;
			_currentMove = Displacement();
			_throwBall = false;
		}

		else if (delta.isDirection() && l <= _selectedPlayer->getSpeed()){
			size_t rest = _selectedPlayer->getSpeed() - l;
			_currentMove.addMove(delta);
			_match.clear(); /* clear hilights */
			
			if (_currentMove.length() < _selectedPlayer->getSpeed()){
				_match.hilightAccessibles(pos, rest);
			}
			_match.hilightDisplacement(_selectedPlayer->getPosition(), _currentMove);
		} else {
			_selectedPlayer = NULL;
			_currentMove = Displacement();
			_throwBall = false;
		}
	}
	redraw();
}

/**
 * Method
 */
void GraphicMatchView::treatTooltip(Position const & pos)
{
	Moveable *atPos = pitch().getAt(pos);
	if (atPos == _tooltipped)
		return;
	_tooltipped = atPos;

	if (! atPos){
		_match.hideTooltip();
	} else {
		std::string text = atPos->getName();
		if (atPos->isPlayer()){
			Player & player = (Player &) *atPos;
			text += " (";
			text += player.getRole() + ")";
			if (isMyPlayer(player)){
				text += "\nClick to select";
				if (player.canQuaffle()){
					PlayerQuaffle & pq = (PlayerQuaffle &) player;
					if (pq.hasQuaffle())
						text += "\nRight-click to throw Quaffle";
				}
			}
		}
		_match.showTooltip(pos+2*Pitch::SouthEast, text);
	}
	redraw();
}

/**
 * Method
 */
bool GraphicMatchView::treatEvent(sf::Event const & ev)
{
	if (ev.type == sf::Event::Closed)
		return false;
	else if (ev.type == sf::Event::GainedFocus)
		redraw();
	else if (ev.type == sf::Event::KeyPressed){
		switch (ev.key.code){
			case sf::Keyboard::Escape: return false;
			case sf::Keyboard::Space: redraw(); break;
			default: break;
		}
	} 

	else if (ev.type == sf::Event::MouseButtonPressed){
		Position GUI_pos = Position(ev.mouseButton.x, ev.mouseButton.y);
		Position pos(_match.GUI2pitch(GUI_pos));
		treatClick(pos, ev.mouseButton.button == sf::Mouse::Left);
	} 

	else if (ev.type == sf::Event::MouseMoved){
		Position GUI_pos = Position(ev.mouseMove.x, ev.mouseMove.y);
		Position pos(_match.GUI2pitch(GUI_pos));
		treatTooltip(pos);
	}
	return true;
}

/**
 * Method handling the gui for a match
 */
void GraphicMatchView::run()
{
	cout << "ENTER GraphicMatchView::run" << endl;
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

	cout << "LEAVE GraphicMatchView::run" << endl;
}

/**
 * Method handling the squads placement / attack direction
 */
void GraphicMatchView::onSquadsInitialized()
{
	_match.setOwnSquadDirection();
}

/**
 * Method handling the refresh for the game surface
 */
void GraphicMatchView::onPitchChange()
{
	redraw();
}

/**
 * Method handling game states (provides shinny infos in the cli)
 */
void GraphicMatchView::onStateChange()
{
	cout << "CHANGE STATE: ";
	switch (state()){
		case CREATED:  cout << "created" << endl; break;
		case PROMPT:   cout << "waiting for moves" << endl; break;
		case TIMEOUT:  cout << "processing moves" << endl; break;
		case FINISHED: 
		default:
			cout << "finished" << endl; break;
	}
}
