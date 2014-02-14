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
	Player *players[7];
	Squad(){memset(players, 0, 7*sizeof(Player*));}
	Squad(JSON::List const & json){
		if (json.len() != 7)
			throw JSON::KeyError("A Squad contains exactly 7 players");
		for (size_t i=0; i<7; i++){
			if (! ISDICT(json[i]))
				throw JSON::TypeError("Expecting JSON::Dict for player");
			players[i] = new Player(DICT(json[i]));
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
		~MatchManager();

		/*!
		 * @meth void MatchManager::addStroke(Stroke const & stroke)
		 * @brief Add a stroke to the current turn
		 */
		void addStroke(Stroke const & stroke);

		/*!
		 * @meth void MatchManager::playStrokes(void)
		 * @brief Play all strokes in this turn
		 */
		void playStrokes(void);

		/*!
		 * @meth void MatchManager::onCollision(Stroke & s1, Stroke & s2, Position &conflict)
		 * @brief Callback called when a collision occurs.
		 * @param s1 Stroke object for the 1st player on the conflicting cell
		 * @param s2 Stroke object for the 2nd player on the conflicting cell
		 * @param conflict The conflicting cell position
		 */
		virtual void onCollision(Stroke & s1, Stroke & s2, Position &conflict){
			std::cout << "Collision " << s1.moveable.getName() << " & "
			          << s2.moveable.getName() << " => " << conflict.toJson();
		}
};

#endif
