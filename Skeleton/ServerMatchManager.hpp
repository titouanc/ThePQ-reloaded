#ifndef ServerMatchManager_hpp
#define ServerMatchManager_hpp

#include "MatchManager.hpp"

class ServerMatchManager : MatchManager {
public:
	void processMoves();
	void sendResolution(Stroke resolution[]);
};

#endif