#ifndef DEFINE_GMATCHMANAGER_HEADER
#define DEFINE_GMATCHMANAGER_HEADER

#include <graphics/UIMatch.hpp>
#include <graphics/GraphicManager.hpp>
#include <stadium/StadiumManager.hpp>
#include <model/Pitch.hpp>
#include "MatchManager.hpp"

class GraphicMatchManager : public MatchManager,  public GUI::GraphicManager {
private:
	UIMatch _match;

	Player *_selectedPlayer; /* Currently selected player, or NULL */
	Displacement _currentMove; /* Displacement for selected player */

	/* Return true if main loop should continue */
	bool treatEvent(sf::Event const & ev);
public:
	GraphicMatchManager(ClientManager const & parent, GUI::MainController & controller); 
	
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
