#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <queue>
#include <pitch/Moveable.hpp>
#include <pitch/Displacement.hpp>
#include <pitch/Pitch.hpp>
#include <cstring>
#include "User.hpp"

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
	Player *players[7];
	Squad(){memset(players, 0, 7*sizeof(Player*));}
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
			players[i] = new Player(DICT(list[i]));
		}
	}
	~Squad(){
		for (size_t i=0; i<7; i++)
			delete players[i];
	}
	JSON::List toJson(){
		JSON::List res;
		for (int i=0; i<7; i++)
			res.append(players[i]->toJson());
		return res;
	}
};

class MatchManager {
	private:
		std::queue<Stroke> _strokes;
		Squad _squads[2];
		Ball *_balls[4];
		Pitch _pitch;
	public:
		MatchManager() : _strokes(), _pitch(100, 36) {
			for (int i=0; i<4; i++)
				_balls[i] = new Ball();
			cout << _pitch;
		}
		~MatchManager(){}

		/*!
		 * @meth void MatchManager::addStroke(Stroke const & stroke)
		 * @brief Add a stroke to the current turn
		 */
		void addStroke(Stroke const & stroke){
			bool found = false;
			for (int s=0; !found && s<2; s++)
				for (int i=0; !found && i<7; i++)
					if (stroke.moveable.getID() == _squads[s].players[i]->getID())
						found = true;
			for (int i=0; !found && i<4; i++)
				if (stroke.moveable.getID() == _balls[i]->getID())
					found = true;
			if (found)
				_strokes.push(stroke);
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
				for (i=0; i<_strokes.size(); i++){
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
