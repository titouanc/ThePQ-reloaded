#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER

#include "ClientManager.hpp"
#include <model/Pitch.hpp>
#include <model/Squad.hpp>
#include <model/Displacement.hpp>

class MatchManager : public ClientManager {
	public:
		typedef enum {CREATED, PROMPT, TIMEOUT, FINISHED} State;
				
	private:
		Pitch _pitch;
		Squad _mySquad, _otherSquad;
		State _state;

		void treatBalls(JSON::List const & balls);
		void treatSquads(JSON::List const & squads);
		void treatDeltas(JSON::List const & delta);

	protected:
		Squad &mySquad() const {return _mySquad};

	public:
		using ClientManager::ClientManager;
		MatchManager(ClientManager const & other);
		virtual ~MatchManager();
		Pitch & pitch() {return _pitch;}
		void treatMessage(JSON::Dict const & msg);

		State state() const {return _state;}

		void invite(std::string const & name);
		void sendDisplacement(Player const & player, Displacement const & move);

		/* HOOKS */
		virtual void onPitchChange(){}
		virtual void onStateChange(){}
};

#endif
