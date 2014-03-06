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
public:
	GraphicMatchManager(
		ClientManager const & parent,
		GUI::MainController & controller
	) : MatchManager(parent), GUI::GraphicManager(controller), _match(pitch()){}
	
	void run();
};

#endif
