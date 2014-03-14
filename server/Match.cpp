#include "Match.hpp"
#include <iostream>

using namespace std;

Match::Match(Squad const & squadA, Squad const & squadB) : 
	_turn(), _finished(false), _hasStroke(20)
{
	_squads[0] = squadA;
	_squads[1] = squadB;
	initMoveables();
	initState();
}

Match::~Match()
{}

void Match::initState()
{
	for (size_t i=0; i<_hasStroke.size(); i++)
		_hasStroke[i] = false;
	_turn.clear();
	_deltas = JSON::List();
}

void Match::initMoveables()
{
	_quaffle.setID(101);
	_snitch.setID(102);
	for (int i=0; i<2; i++){
		for (int j=0; j<7; j++){
			/* Players IDs: 1..7 and 11..16*/
			_squads[i].players[j]->setID(10*i+j+1);
		}
		_bludgers[i].setID(103+i);
	}

	Position const & c = _pitch.center();
	_quaffle.setPosition(c);
	_pitch.insert(&_quaffle);

	for (int i=0; i<2; i++){
		int k = i ? 2 : -2; /* [0] -> East; [1] -> West */

		_bludgers[i].setPosition(c + 6*k*Pitch::NorthWest);
		_pitch.insert(_bludgers+i);

		_squads[i].chasers[0].setPosition(c + k*Pitch::West);
		_squads[i].chasers[1].setPosition(c + 2*k*Pitch::NorthWest);
		_squads[i].chasers[2].setPosition(c + 2*k*Pitch::SouthWest);
		_squads[i].beaters[0].setPosition(c + k*Pitch::West + k*Pitch::SouthWest);
		_squads[i].beaters[1].setPosition(c + k*Pitch::West + k*Pitch::NorthWest);
		_squads[i].seeker.setPosition(c + 2*k*Pitch::West);
		_squads[i].keeper.setPosition(c + 9*k*Pitch::West);
		for (int j=0; j<7; j++)
			_pitch.insert(_squads[i].players[j]);
	}

	_snitch.setPosition(c + 6*Pitch::SouthWest);
	_pitch.insert(&_snitch);
}

size_t Match::timesteps() const
{
	size_t res = 0;
	for (Stroke const & stroke : _turn)
		if (stroke.move.length() > res)
			res = stroke.move.length();
	return res;
}

Moveable *Match::id2Moveable(int mid)
{
	Squad *inSquad = id2Squad(mid);
	if (inSquad)
		return inSquad->players[(mid%10)-1];
	else if (mid == 101)
		return &_quaffle;
	else if (mid == 102)
		return &_snitch;
	else if (103 <= mid && mid <= 104)
		return &(_bludgers[mid-103]);
	return NULL;
}

Squad *Match::id2Squad(int mid)
{
	if (1 <= mid && mid <= 7)
		return &(_squads[0]);
	else if (11 <= mid && mid <= 17)
		return &(_squads[1]);
	return NULL;
}

std::pair<std::string, unsigned int> Match::getWinner() const
{
	if (_points[0] > _points[1])
		return pair<string, unsigned int>(_squads[0].squad_owner, _points[0]);
	return std::pair<string, unsigned int>(_squads[0].squad_owner, _points[1]);
}

std::pair<std::string, unsigned int> Match::getLoser() const
{
	if (_points[0] <= _points[1])
		return pair<string, unsigned int>(_squads[0].squad_owner, _points[0]);
	return std::pair<string, unsigned int>(_squads[0].squad_owner, _points[1]);
}

std::list<Stroke>::iterator Match::getStrokeFor(Moveable const & moveable)
{
	std::list<Stroke>::iterator res;
	for (res=_turn.begin(); res!=_turn.end(); res++){
		if (res->moveable.getID() == moveable.getID())
			break;
	}
	return res;
}

bool Match::addStroke(Stroke const & stroke)
{
	if (! id2Moveable(stroke.moveable.getID()))
		return false;
	_turn.push_back(stroke);
	return true;
}

bool Match::addStroke(int mid, Displacement const & d)
{
	Moveable *moveable = id2Moveable(mid);
	if (! moveable)
		return false;
	addStroke(Stroke(*moveable, d));
	return true;
}

JSON::List Match::playStrokes()
{
	/* This method process moveables moves, in regular time intervals. The
	   Moveable are moved on the pitch structure, and their positions attributes
	   are updated at the end. */

	size_t n_steps = timesteps();
	double tLast = 0;
	double step = 1.0/n_steps;

	/* for each interval of time in this turn do... */
	for (size_t i=1; i<=n_steps; i++){
		_t = step*i;

		/* for each stroke do... */
		for (Stroke & stroke : _turn){
			cout << "\t" << JSON::Dict(stroke) << endl;
			if (! stroke.active){
				/* The moveable with this stroke has been stopped */
				continue;
			}

			/* Current position for the moveable of this stroke */
			Position curPos  = stroke.move.position(tLast, stroke.moveable);
			Position nextPos = stroke.move.position(_t, stroke.moveable);

			cout << "\t" << JSON::List(curPos) << "->" << JSON::List(nextPos) << endl;

			if (curPos == nextPos){
				/* If the moveable Position doesn't change during this
				   time interval, just skip */
				continue;
			}

			Collision collide(curPos, nextPos, stroke);

			if (! (
				stayInEllipsis(collide) ||
				keeperInZone(collide) ||
				playerCatchQuaffle(collide) ||
				scoreGoal(collide)
			)){
				/* If no rule has stopped this stroke; set it to its new position */
				_pitch.setAt(nextPos, &(stroke.moveable));
				_pitch.setAt(curPos, NULL);
			}
		}
		tLast = _t;
	}

	/* Generate delta for unstopped players */
	for (Stroke & stroke : _turn){
		if (! stroke.active)
			continue;
		JSON::Dict d = {
			{"mid", JSON::Integer(stroke.moveable.getID())},
			{"from", JSON::List(stroke.moveable.getPosition())},
			{"to", JSON::List(stroke.move.position(1, stroke.moveable))}
		};
		_deltas.append(d);
	}

	/* Update all players positions attributes that already have a delta */
	for (std::pair<Position const &, Moveable*> it : _pitch){
		it.second->setPosition(it.first);
	}

	JSON::List res = _deltas;
	initState();
	return res;
}

bool Match::isFinished() const
{
	return _finished;
}

/* ================== RULES ================= */
bool Match::stayInEllipsis(Collision & collide)
{
	if (_pitch.inEllipsis(collide.conflict))
		return false;

	cout << collide.stroke.moveable.getName() << " GETS OUT !!!" << endl;

	collide.stroke.active = false;
	return true;
}

bool Match::keeperInZone(Collision & collide)
{
	if (! collide.stroke.moveable.isPlayer())
		return false;

	Player & player = (Player &) collide.stroke.moveable;
	/* If it isn't a keeper, or still in keeper zone, do nothing */
	if (! player.isKeeper() || _pitch.isInKeeperZone(collide.conflict))
		return false;

	cout << collide.stroke.moveable.getName() << " IN KEEPER ZONE !!!" << endl;

	/* Otherwise stop the move */
	collide.stroke.active = false;
	return true;
}

bool Match::scoreGoal(Collision & collide)
{
	if (! _pitch.isGoal(collide.conflict))
		return false;

	if (collide.stroke.moveable.isBall()){
		Ball & ball = (Ball &) collide.stroke.moveable;
		if (! ball.isQuaffle())
			return false;

		/* Give 10 points to the squad who started on the opposite side 
		   of this goal */
		if (_pitch.isInWestKeeperZone(collide.conflict)){
			_points[0] += 10; /* see initMoveables() for initial squads positions */
		} else {
			_points[1] += 10;
		}
		return false;
	} 

	Player & player = (Player &) (collide.stroke.moveable);
	if (! player.isChaser())
		return false;

	Chaser & chaser = (Chaser &) player;
	if (chaser.hasQuaffle()){
		/* If it is a chaser, and he has the ball; let him score */
		Squad *owner = id2Squad(chaser.getID());
		if (owner == &(_squads[0]) && _pitch.isInWestKeeperZone(collide.conflict)){
			_points[0] += 10;
			return false;
		}

		if (owner == &(_squads[1]) && _pitch.isInEastKeeperZone(collide.conflict)){
			_points[1] += 10;
			return false;
		}
	}

	/* The Chaser try to go through its own goal; STAHP ! NOW ! */
	collide.stroke.active = false;
	return true;
}

/* This rule give the Quaffle to a chaser||keeper */
bool Match::playerCatchQuaffle(Collision & collide)
{
	Moveable *atPos = _pitch.getAt(collide.conflict);
	if (atPos == NULL)
		return false;

	/* The moveable is the quaffle, check if the other is a PlayerQuaffle */
	if (&(collide.stroke.moveable) == &_quaffle){
		if (! atPos->isPlayer())
			return false;
		
		Player & player = (Player &) *atPos;
		if (! player.canQuaffle())
			return false;
		
		/* If yes, stop its movement and give it to the player */
		PlayerQuaffle & pq = (PlayerQuaffle &) player;
		pq.retainQuaffle();
		collide.stroke.active = false;

		/* And remove the Quaffle from the pitch */
		_pitch.setAt(collide.fromPos, NULL);
		return true;
	}

	/* The moveable is a player; is this a PlayerQuaffle ? 
	   Is the other a Quaffle ? */
	else if (collide.stroke.moveable.isPlayer()){
		Player & player = (Player &) collide.stroke.moveable;
		if (! player.isChaser() || atPos != &_quaffle)
			return false;

		/* If yes, give it the Quaffle */
		PlayerQuaffle & pq = (PlayerQuaffle &) player;
		pq.retainQuaffle();

		/* Find the stroke for the Quaffle, if any, and deactivate it */
		for (Stroke & stroke : _turn){
			if (&(stroke.moveable) == &_quaffle){
				stroke.active = false;
				break;
			}
		}

		/* Put the player on the position of the Quaffle */
		_pitch.setAt(collide.conflict, &pq);
		_pitch.setAt(collide.fromPos, NULL);

		return true;
	}

	return false;
}
