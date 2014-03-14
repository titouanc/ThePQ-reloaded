#ifndef DEFINE_MATCH_HEADER
#define DEFINE_MATCH_HEADER

#include <list>
#include <vector>
#include <utility>
#include <model/Stroke.hpp>
#include <model/Pitch.hpp>
#include <model/Ball.hpp>
#include <model/Displacement.hpp>
#include <model/Squad.hpp>

/* Represent a potential collision if a stroke move a player from a certain
   position to another. */
struct Collision {
	/*! Where the collision happens */
	Position const & conflict;
	/*! Last position occupied by the secon moveable to arrive */
	Position const & fromPos;
	/*! Current stroke */
	Stroke & stroke;
	Collision(Position const & c, Position const & f, Stroke & s) : 
		conflict(c), fromPos(f), stroke(s) 
	{}
};

class Match {
private:
	/*! strokes for this turn */
	std::list<Stroke> _turn;

	/*! what changed during this turn */
	JSON::List _deltas;

	/*! Scores */
	int _points[2];

	/*! Player -> position lookup */
	Squad _squads[2];
	Bludger _bludgers[2];
	Quaffle _quaffle;
	GoldenSnitch _snitch;

	/*! Position -> moveable lookup */
	Pitch _pitch;

	bool _finished;
	std::vector<bool> _hasStroke;

	/*! current time step while playing moves */
	double _t;

	/*! Initialize an empty turn (clear all strokes, hasStroke flags and deltas) */
	void initState();

	/*! Initialize players positions and ID */
	void initMoveables();

	/*! Return the number of timesteps to consider (longest displacement) */
	size_t timesteps() const;

	std::list<Stroke>::iterator getStrokeFor(Moveable const & moveable);

	/* RULES: method accepting a Collision object.
	          Return true if the rule processing should stop */
	
	/*! All Moveables stay on the pitch */
	bool stayInEllipsis(Collision & collide);

	/*! A keeper could not get out of his zone */
	bool keeperInZone(Collision & collide);
	
	/*! A Chaser can catch the ball */
	bool playerCatchQuaffle(Collision & collide);

	/*! A Chaser or Quaffle can pass in the goal */
	bool scoreGoal(Collision & collide);

public:
	Match(Squad const & squad1, Squad const & squad2);
	~Match();

	/*! Return a pointer to moveable, given its ID 
		@return NULL if no moveable maps to this mid
	 */
	Moveable *id2Moveable(int mid);

	/*! Return a squad containing the Player with ID mid.
	    @return NULL if no moveable maps to this mid or it is a Ball
	 */
	Squad *id2Squad(int mid);

	/*! Return a pair <"username",score> */
	std::pair<std::string, unsigned int> getWinner() const;
	std::pair<std::string, unsigned int> getLoser() const;

	/*! add a stroke for this turn */
	bool addStroke(Stroke const & stroke);
	bool addStroke(int mid, Displacement const & d);

	/*! Play all strokes, and return a list of match changes
	    @return [{"mid": <int>, "from": <pos>, "to": <pos>}], where pos is a
	            Position object serialized as JSON::List.
	 */
	JSON::List playStrokes();

	/*! Return true if the golden snitch has been catched */
	bool isFinished() const;
};

#endif
