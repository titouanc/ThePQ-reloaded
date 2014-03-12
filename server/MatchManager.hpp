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
#include <model/User.hpp>
#include <network/ConnectionManager.hpp>
#include <model/Player.hpp>
#include <model/Squad.hpp>
#include <model/MatchResult.hpp>
#include <Constants.hpp>
#include <network/SubConnectionManager.hpp>

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
		bool _champMatch;
		struct MatchResult _matchRes;
		/* Current match deltas */
		JSON::List _turnDeltas;

		/* Current time step */
		double _t;

		typedef std::deque<Stroke>::iterator iter;

		/* initialise moveable positions */
		void initPositions(void);

		/* Processing of incoming messages*/
		void processMessage(Message const & msg);
		/* Process incoming message with given data (a JSON::Dict)
		   Could not be made by Stroke(JSON::Dict) because we have to map
		   moveable_id -> Moveable */
		void processStroke(Message const & msg, JSON::Dict const & data);
		/* Create random moves for golden snitch */
		void mkSnitchStroke(void);

		/* Send messages to clients */
		void sendToAll(JSON::Value const & data);
		/* Send {"type": "<signal>", "data": true} to everyone */
		void sendSignal(std::string const & sig);
		/* Send message to <msg> sender with given <type> and payload */
		void reply(Message const & msg, std::string type, JSON::Value const & data);
		/* Like ^ but data set to "<text>" */
		void reply(Message const & msg, std::string type, const char *text);
		/* Send squads composition to everyone */
		void sendMoveables(void);
		/* Send match delta to everyone */
		void sendMatchDeltas(void);
		void stopStroke(Stroke & stroke, Position const & pos);
		void addDelta(Moveable const & moveable, Position const & dest);
		void resolveFame(std::string win,std::string los);
		void resolveMoney(std::string win,std::string los);
		iter getStrokeForMoveable(Moveable *moveable);
		/* Resolve strokes */
		void playStrokes(void);
		bool checkGoal(
			Stroke & stroke,     /* Stroke that might lead to goal */
			Position & toPos,    /* Position that might be a goal */
			Position & fromPos  /* last pos occupied by moving */
		);
		/* *SMASH* */
		void onCollision(
			Stroke & stroke,     /* Stroke that leads to conflict */
			Position & conflict, /* Clonflicting pos */
			Position & fromPos  /* last pos occupied by moving */
		);
		void throwBall(
			Moveable & ball,
			Position & fromPos,
			Position & direction
		);
		void endMatch(void);
	public:
		MatchManager(
			BaseConnectionManager & connections, 
			Squad const & squadA, Squad const & squadB,
			bool champMatch
		);
		virtual ~MatchManager();
		/* Run dat shit */
		void _mainloop_out();
		bool isChampMatch();
		MatchResult& getResult();
};

#endif
