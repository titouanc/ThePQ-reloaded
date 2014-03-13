#include "Match.hpp"
#include <iostream>

using namespace std;

Match::Match(Squad const & squadA, Squad const & squadB) : 
	_finished(false), _hasStroke(20)
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

void Match::addStroke(Stroke const & stroke)
{
	_turn.push_back(stroke);
}

JSON::List Match::playStrokes()
{
	size_t n_steps = timesteps();
	double tLast = 0;
	double step = 1.0/n_steps;

	/* for each interval of time in this turn do... */
	for (size_t i=1; i<=n_steps; i++){
		_t = step*i;

		/* for each stroke do... */
		for (Stroke & stroke : _turn){
			if (! stroke.active){
				/* The moveable with this stroke has been stopped */
				continue;
			}

			/* Current position for the moveable of this stroke */
			Position curPos  = stroke.move.position(_t, stroke.moveable);
			Position nextPos = stroke.move.position(tLast, stroke.moveable);

			if (curPos == nextPos){
				/* If the moveable Position doesn't change during this
				   time interval, just skip */
				continue;
			}

			/* Is there someone (or something) at the position the 
			   moveable would occupy ? */
			Moveable *atDest = _pitch.getAt(nextPos);
			
			if (atDest == NULL){
				_pitch.setAt(nextPos, &(stroke.moveable));
				_pitch.setAt(curPos, NULL);
				cout << "MOVE " << JSON::List(curPos) << " -> " << JSON::List(nextPos) << endl;
			} else {
				//onCollision();
				cout << "COLLISION " << JSON::List(nextPos) << endl;
				stroke.active = false;
			}
		}
		tLast = _t;
	}

	JSON::List res = _deltas;
	initState();
	return res;
}

bool Match::isFinished() const
{
	return _finished;
}
