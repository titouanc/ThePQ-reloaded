#ifndef DEFINE_GMATCHMANAGER_HEADER
#define DEFINE_GMATCHMANAGER_HEADER

#include <graphics/UIMatch.hpp>
#include <graphics/GraphicManager.hpp>
#include <stadium/StadiumController.hpp>
#include <model/Pitch.hpp>
#include "MatchController.hpp"

class GraphicMatchView : public MatchController,  public GUI::GraphicManager {
private:
	UIMatch _match;

	Player *_selectedPlayer; //!< Currently selected player, or NULL
	Displacement _currentMove; //!< Displacement for selected player
	bool _throwBall;  //!< Throw ball action requested (right click)
	Moveable *_tooltipped;      //!< The tooltip is on this moveable

	void treatClick(Position const & pos, bool regularClick);

	void treatTooltip(Position const & pos);

	/* Return true if main loop should continue */
	bool treatEvent(sf::Event const & ev);
public:
	GraphicMatchView(ClientController const & parent, GUI::MainController & controller); 
	virtual ~GraphicMatchView();
	
	/* Redraw pitch on window */
	void redraw();

	/* Run mainloop */
	void run();

	/* Triggered when the squads are initialized at the beginning of the game */
	void onSquadsInitialized();

	/* Triggered when the pitch changed */
	void onPitchChange();

	/* Triggered when match state changes */
	void onStateChange();
};

#endif
