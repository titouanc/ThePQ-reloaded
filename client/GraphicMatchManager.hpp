#ifndef DEFINE_GMATCHMANAGER_HEADER
#define DEFINE_GMATCHMANAGER_HEADER

#include "graphics/UIMatch.hpp"
#include "graphics/GraphicManager.hpp"
#include "StadiumManager.hpp"
#include <model/Pitch.hpp>
#include "MatchManager.hpp"

class GraphicMatchManager : public MatchManager,  public GUI::GraphicManager {
private:
	UIMatch _match;

	/* Return true if main loop should continue */
	bool treatEvent(sf::Event const & ev);
public:
	GraphicMatchManager(
		ClientManager const & parent,
		GUI::MainController & controller
	) : MatchManager(parent), GUI::GraphicManager(controller), _match(pitch()){}
	
	/* Redraw pitch on window */
	void redraw();

	/* Run mainloop */
	void run();

	/* Triggered when the pitch changed */
	void onPitchChange();

	/* Triggered when match state changes */
	void onStateChange();
};

#endif
