#ifndef ServerMatchManager_hpp
#define ServerMatchManager_hpp

#include "MatchManager.h"

class ServerMatchManager : MatchManager {
public:
	void processMoves();
	void sendResolution(Stroke resolution[]);
};

#endif