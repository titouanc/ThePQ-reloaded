#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <cassert>

#include <sys/select.h>
#include <queue>
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

using namespace net;

struct Stroke {
	Moveable & moveable;
	Displacement move;
	Stroke(Moveable & m, Displacement d) : moveable(m), move(d){}
};

/* TODO: mettre ca dans un fichier de constantes */
#define MATCH_START   "MSTART"
#define MATCH_END     "MEND"
#define MATCH_SQUADS  "MSQUADS"
#define MATCH_PROMPT  "M?"
#define MATCH_TIMEOUT "MTOUT"
#define MATCH_STROKE  "MSTROKE"
#define MATCH_ERROR   "M!!!"
#define MATCH_ACK     "MACK"

class MatchManager {
	private:
		std::queue<Stroke> _strokes;
		Squad _squads[2];
		Ball   _balls[4];
		Pitch  _pitch;
		SharedQueue<Message> _inbox, _outbox;
		SubConnectionManager _net;

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

		/* Resolve strokes */
		void playStrokes(void);
		/* *SMASH* */
		void onCollision(double t, Stroke & s, Position &conflict);
	public:
		MatchManager(
			BaseConnectionManager & connections, 
			Squad const & squadA, Squad const & squadB
		);
		~MatchManager();
		/* Run dat shit */
		void run();
};

#endif
