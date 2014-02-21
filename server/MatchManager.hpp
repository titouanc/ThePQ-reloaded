#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <cassert>

#include <sys/select.h>
#include <deque>
#include <model/Moveable.hpp>
#include <model/Displacement.hpp>
#include <model/Pitch.hpp>
#include <model/Player.hpp>
#include <model/Ball.hpp>
#include <cstring>
#include "User.hpp"
#include <network/ConnectionManager.hpp>
#include <model/Player.hpp>
#include <model/Squad.hpp>
#include <Constants.hpp>

using namespace net;

struct Stroke {
	Moveable & moveable;
	Displacement move;
	bool active;
	Stroke(Moveable & m, Displacement d) : moveable(m), move(d), active(true){}
};

class MatchManager : public SubConnectionManager {
	private:
		std::deque<Stroke> _strokes;
		/* Players */
		Squad _squads[2];
		/* Balls */
		Quaffle _quaffle;
		GoldenSnitch _snitch;
		Bludger _bludgers[2];
		Pitch  _pitch;
		SharedQueue<Message> _inbox, _outbox;
		unsigned int _score[2];

		typedef std::deque<Stroke>::iterator iter;

		/* initialise moveable positions */
		void initPositions(void);

		/* Sleep with a microsecond resolution */
		void minisleep(double secs);

		/* Processing of incoming messages*/
		void processMessage(Message const & msg);
		/* Process incoming message with given data (a JSON::Dict)
		   Could not be made by Stroke(JSON::Dict) because we have to map
		   moveable_id -> Moveable */
		void processStroke(Message const & msg, JSON::Dict const & data);

		/* Send messages to clients */
		void sendToAll(JSON::Value const & data);
		/* Send {"type": "<signal>", "data": true} to everyone */
		void sendSignal(std::string const & sig);
		/* Send message to <msg> sender with given <type> and payload */
		void reply(Message const & msg, std::string type, JSON::Value const & data);
		/* Like ^ but data set to "<text>" */
		void reply(Message const & msg, std::string type, const char *text);
		/* Send squads composition to everyone */
		void sendSquads(void);
		void sendBalls(void);
		/* Send match delta to everyone */
		void sendMatchDeltas(JSON::List const & delta);

		iter getStrokeForMoveable(Moveable *moveable);
		/* Resolve strokes */
		void playStrokes(void);
		bool checkGoal(
			Stroke & stroke,     /* Stroke that might lead to goal */
			Position & toPos,    /* Position that might be a goal */
			Position & fromPos,  /* last pos occupied by moving */
			JSON::List & deltas  /* Where to save match deltas */
		);
		/* *SMASH* */
		void onCollision(
			Stroke & stroke,     /* Stroke that leads to conflict */
			Position & conflict, /* Clonflicting pos */
			Position & fromPos,  /* last pos occupied by moving */
			JSON::List & deltas  /* Where to save match deltas */
		);
	public:
		MatchManager(
			BaseConnectionManager & connections, 
			Squad const & squadA, Squad const & squadB
		);
		virtual ~MatchManager();
		/* Run dat shit */
		void _mainloop_out();
};

#endif
