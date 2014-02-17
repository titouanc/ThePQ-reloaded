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

using namespace net;

struct Stroke {
	Moveable & moveable;
	Displacement move;
	Stroke(Moveable & m, Displacement d) : moveable(m), move(d){}
};

struct Squad {
	int squad_id;
	int client_id;
	Chaser chasers[3];
	Beater beaters[2];
	Seeker seeker;
	Keeper keeper;
	Player *players[7];

	Squad(){
		for (int i=0; i<3; i++)
			players[i] = &(chasers[i]);
		for (int i=0; i<2; i++)
			players[i+3] = &(beaters[i]);
		players[5] = &seeker;
		players[6] = &keeper;
	}
	Squad(JSON::Dict const & json){
		if (ISLIST(json.get("chasers")) && LIST(json.get("chasers")).len() == 3){
			JSON::List const & list = LIST(json.get("chasers"));
			for (int i=0; i<3; i++)
				chasers[i] = Chaser(DICT(list[i]));
		}
		if (ISLIST(json.get("beaters")) && LIST(json.get("beaters")).len() == 2){
			JSON::List const & list = LIST(json.get("beaters"));
			for (int i=0; i<2; i++)
				beaters[i] = Beater(DICT(list[i]));
		}
		if (ISDICT(json.get("seeker")))
			seeker = Seeker(DICT(json.get("seeker")));
		if (ISDICT(json.get("keeper")))
			keeper = Keeper(DICT(json.get("keeper")));

	}
	operator JSON::Dict(){
		JSON::Dict res;
		res.set("squad_id", squad_id);
		res.set("seeker", JSON::Dict(seeker));
		res.set("keeper", JSON::Dict(keeper));

		JSON::List b;
		for (int i=0; i<2; i++)
			b.append(JSON::Dict(beaters[i]));
		res.set("beaters", b);

		JSON::List c;
		for (int i=0; i<3; i++)
			c.append(JSON::Dict(chasers[i]));
		res.set("chasers", c);
		return res;
	}
};

/* TODO: mettre ca dans un fichier de constantes */
#define MATCH_START   "MSTART"
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
	public:
		MatchManager(
			BaseConnectionManager & connections, 
			Squad const & squadA, Squad const & squadB
		) : _strokes(), _pitch(100, 36), _inbox(), _outbox(), 
		   _net(_inbox, _outbox, connections)
		{
			for (int i=0; i<4; i++)
				_balls[i].setID(100+i+1); /* Balls IDs: 101..104*/

			_squads[0] = squadA;
			_squads[1] = squadB;

			for (int i=0; i<2; i++){
				for (int j=0; j<7; j++){
					/* Players IDs: 1..7 and 11..16*/
					_squads[i].players[j]->setID(10*i+j+1);
				}
			}

			/* Get a local communication channel */
			_net.acquireClient(squadA.client_id);
			_net.acquireClient(squadB.client_id);
			_net.start();
		}
		~MatchManager()
		{
			while (_outbox.available());
			std::cout << "GOT HERE" << endl;
		}

		void minisleep(double secs)
		{
			assert(secs >= 0);
			int seconds = secs;
			int micros  = secs*1000000 - seconds;

			struct timeval interval = {seconds, micros};
			select(1, NULL, NULL, NULL, &interval);
		}

		void processStroke(Message const & msg, JSON::Dict const & data)
		{
			if (! ISINT(data.get("moveable_id")))
				return reply(msg, MATCH_ERROR, "Missing moveable ID");
			int mid = INT(data.get("moveable_id"));
			if (mid < 1 || (mid < 11 && mid > 7) || mid > 17)
				return reply(msg, MATCH_ERROR, "Not a player");
			int squad = mid/10;
			int player = mid%10;

			if (msg.peer_id != _squads[squad].client_id)
				return reply(msg, MATCH_ERROR, "Not your player");

			if (! ISLIST(data.get("move")))
				return reply(msg, MATCH_ERROR, "No displacement found");
			
			_strokes.push(Stroke(
				*(_squads[squad].players[player]), 
				Displacement(LIST(data.get("move")))
			));
			reply(msg, MATCH_ACK, data);
		}

		void processMessage(Message const & msg)
		{
			/* Find if message come from one of known clients */
			Squad *sender = NULL;
			for (int i=0; i<2; i++)
				if (msg.peer_id == _squads[i].client_id)
					sender = &(_squads[i]);
			if (! sender){
				return reply(msg, MATCH_ERROR, "Not in allowed users");
			}

			JSON::Dict const & data = DICT(msg.data);
			if (! ISSTR(data.get("type")))
				return reply(msg, MATCH_ERROR, "Missing \"type\":string");
			if (STR(data.get("type")).value() == MATCH_STROKE){
				if (! ISDICT(data.get("data")))
					return reply(msg, MATCH_ERROR, "Data should be a dict");
				processStroke(msg, DICT(data.get("data")));
			}
		}

		void run(){
			sendSquads();
			sendSignal(MATCH_START);
			time_t tick;

			while (_net.nClients() > 0){
				sendSignal(MATCH_PROMPT);
				tick = time(NULL);

				do {
					if (_inbox.available()){
						Message const & msg = _inbox.pop();
						if (ISDICT(msg.data))
							processMessage(msg);
						else
							reply(msg, MATCH_ERROR, "Not a dict");
						delete msg.data;
					} else {
						minisleep(0.1);
					}
				} while (time(NULL) - tick < 30);
				
				sendSignal(MATCH_TIMEOUT);
				playStrokes();
			}
		}

		/* send data to both clients */
		void sendToAll(JSON::Value const & data){
			for (int i=0; i<2; i++)
				_outbox.push(Message(_squads[i].client_id, data.clone()));
		}

		/* send {"type": "<sig>", "data": true} to both clients */
		void sendSignal(std::string const & sig){
			JSON::Dict msg;
			msg.set("type", sig);
			msg.set("data", JSON::Bool(true));
			sendToAll(msg);
		}

		void reply(Message const & msg, std::string type, JSON::Value const & data){
			JSON::Dict *response = new JSON::Dict();
			response->set("type", type);
			response->set("data", data);
			_outbox.push(Message(msg.peer_id, response));
		}

		void reply(Message const & msg, std::string type, const char *text){
			reply(msg, type, JSON::String(text));
		}

		/* send squads composition */
		void sendSquads(void){
			int i;
			JSON::List *list = new JSON::List();
			for (i=0; i<2; i++)
				list->append(JSON::Dict(_squads[i]));
			
			JSON::Dict msg;
			msg.setPtr("squads", list);
			msg.set("type", MATCH_SQUADS);
			sendToAll(msg);
		}

		/*!
		 * @meth void MatchManager::playStrokes(void)
		 * @brief Play all strokes in this turn
		 */
		void playStrokes(void){
			size_t i, len, maxlen=0;

			/* Find longest displacement */
			for (i=0; i<_strokes.size(); i++){
				Stroke &stroke = _strokes.front();
				_strokes.pop();
				_strokes.push(stroke);
				len = stroke.move.length();
				if (len > maxlen)
					maxlen = len;
			}

			/* For each step in time interval... */
			for (double t=1.0/maxlen; t<=1; t+=1.0/maxlen){
				size_t n_strokes = _strokes.size();
				/* For each stroke */
				for (i=0; i<n_strokes; i++){
					Stroke &stroke = _strokes.front();
					_strokes.pop();
					_strokes.push(stroke);
					
					/* Where the moveable would be */
					Position newPos = stroke.move.position(t, stroke.moveable);
					/* Who's there ? */
					Moveable *atPos = _pitch.getAt(newPos);
					if (atPos)
						onCollision(stroke, newPos);
					else
						_pitch.setAt(newPos, &(stroke.moveable));
				}
			}

			while (! _strokes.empty())
				_strokes.pop();
		}

		/*!
		 * @meth void MatchManager::onCollision(Stroke & s, Position &conflict)
		 * @brief Callback called when a collision occurs.
		 * @param s Stroke object for the 2nd player on the conflicting cell
		 * @param conflict The conflicting cell position
		 */
		virtual void onCollision(Stroke & s, Position &conflict)
		{
			std::cout << "Collision " << s.moveable.getName() << " & "
			          << _pitch.getAt(conflict)->getName() 
			          << " => " << conflict.toJson() << std::endl;
		}
};

#endif
