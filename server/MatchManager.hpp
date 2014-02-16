#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <queue>
#include <model/Moveable.hpp>
#include <model/Displacement.hpp>
#include <model/Pitch.hpp>
#include <cstring>
#include "User.hpp"
#include <network/ConnectionManager.hpp>
#include <network/TcpSocket.hpp>

using namespace net;

class Player : public Moveable {
	using Moveable::Moveable;
};

class Ball : public Moveable {
	using Moveable::Moveable;
};

struct Stroke {
	Moveable & moveable;
	Displacement move;
};

struct Squad {
	int squad_id;
	int client_id;
	Player players[7];
	Squad(){}
	Squad(JSON::Dict const & json){
		if (! ISINT(json.get("squad_id")))
			throw JSON::KeyError("A squad should have an ID");
		if (! ISLIST(json.get("players")))
			throw JSON::KeyError("A squad should contain players");

		JSON::List const & list = LIST(json.get("players"));
		if (list.len() != 7)
			throw JSON::KeyError("A Squad should contain exactly 7 players");
		for (size_t i=0; i<7; i++){
			if (! ISDICT(list[i]))
				throw JSON::TypeError("Expecting JSON::Dict for player");
			players[i] = Player(DICT(list[i]));
		}
	}
	JSON::Dict toJson(){
		JSON::List list;
		for (int i=0; i<7; i++)
			list.append(players[i].toJson());
		JSON::Dict res;
		res.set("players", list);
		res.set("squad_id", squad_id);
		return res;
	}
	operator JSON::Dict(){return toJson();}
};

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
					_squads[i].players[j].setID(10*i+j+1);
				}
			}

			_net.acquireClient(squadA.client_id);
			_net.acquireClient(squadB.client_id);
			_net.start();
		}
		~MatchManager(){}

		void run(){
			sendSquads();
		}

		void sendSquads(void){
			int i;
			JSON::List *list = new JSON::List();
			for (i=0; i<2; i++)
				list->append(_squads[i].toJson());
			
			JSON::Dict msgdata;
			msgdata.setPtr("squads", list);
			msgdata.set("type", MSG::MATCH_SQUADS);

			for (i=0; i<2; i++)
				_outbox.push(Message(_squads[i].client_id, msgdata.clone()));
		}

		/*!
		 * @meth void MatchManager::playStrokes(void)
		 * @brief Play all strokes in this turn
		 */
		void playStrokes(void){
			size_t i, len, maxlen=0;

			for (i=0; i<_strokes.size(); i++){
				Stroke &stroke = _strokes.front();
				_strokes.pop();
				_strokes.push(stroke);
				len = stroke.move.length();
				if (len > maxlen)
					maxlen = len;
			}

			for (double t=1.0/maxlen; t<=1; t+=1.0/maxlen){
				size_t n_strokes = _strokes.size();
				for (i=0; i<n_strokes; i++){
					Stroke &stroke = _strokes.front();
					_strokes.pop();
					_strokes.push(stroke);
					Position newPos = stroke.move.position(t, stroke.moveable);
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
		virtual void onCollision(Stroke & s, Position &conflict){
			std::cout << "Collision " << s.moveable.getName() << " & "
			          << _pitch.getAt(conflict)->getName() 
			          << " => " << conflict.toJson() << std::endl;
		}
};

#endif
