#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER

#include "ClientManager.hpp"
#include <model/Pitch.hpp>

class MatchManager : public ClientManager {
	private:
		Pitch _pitch;
	public:
		using ClientManager::ClientManager;
		MatchManager(ClientManager const & other) : ClientManager(other){}
		Pitch & pitch() {return _pitch;}
};

#endif
