#ifndef DEFINE_MATCH_HEADER
#define DEFINE_MATCH_HEADER

#include <deque>
#include <vector>
#include <utility>
#include <model/Stroke.hpp>
#include <model/Pitch.hpp>
#include <model/Ball.hpp>
#include <model/Displacement.hpp>
#include <model/Squad.hpp>

class Match {
private:
	/*! strokes for this turn */
	std::deque<Stroke> _turn;

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
	void addStroke(Stroke const & stroke);

	/*! Play all strokes, and return a list of match changes
	    @return [{"mid": <int>, "from": <pos>, "to": <pos>}], where pos is a
	            Position object serialized as JSON::List.
	 */
	JSON::List playStrokes();

	/*! Return true if the golden snitch has been catched */
	bool isFinished() const;
};

#endif
