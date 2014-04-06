#include "Match.hpp"
#include <iostream>
#include <cassert>

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

Squad const & Match::squad(int id)
{
	assert(id==0 || id==1);
	return _squads[id];
}

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

	_snitch.setPosition(c + 6*Pitch::SouthWest+6*Pitch::SouthEast);
	_pitch.insert(&_snitch);
}

size_t Match::timesteps() const
{
	size_t res = 0;/*
	for (Stroke const & stroke : _turn)
		if (stroke.move.length() > res)
			res = stroke.move.length();*/
	for (size_t i=0; i<7; i++){
		for (int j=0; j<2; j++)
			if (_squads[j].players[i]->getSpeed() > res)
				res = _squads[j].players[i]->getSpeed();
	}
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
	return std::pair<string, unsigned int>(_squads[1].squad_owner, _points[1]);
}

std::pair<std::string, unsigned int> Match::getLoser() const
{
	if (_points[0] <= _points[1])
		return pair<string, unsigned int>(_squads[0].squad_owner, _points[0]);
	return std::pair<string, unsigned int>(_squads[1].squad_owner, _points[1]);
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

bool Match::addStroke(
	int mid, 
	Displacement const & d, 
	ActionType act,
	Position actpos,
	Position actvec
){
	Moveable *moveable = id2Moveable(mid);
	if (! moveable)
		return false;
	addStroke(Stroke(*moveable, d, act, actpos, actvec));
	return true;
}

bool Match::addStroke(JSON::Dict const & stroke){
	if (ISINT(stroke.get("mid"))){
		Moveable *moving = id2Moveable(INT(stroke.get("mid")));
		if (moving)
			return addStroke(Stroke(*moving, stroke));
	}
	return false;
}

/* Helper function to quickly create DELTA_MOVE JSON objects */
static JSON::Dict mkDelta(Moveable const & moveable, Position destPos)
{
	JSON::Dict res = {
		{"type", JSON::Integer(DELTA_MOVE)},
		{"mid", JSON::Integer(moveable.getID())},
		{"from", JSON::List(moveable.getPosition())},
		{"to", JSON::List(destPos)}
	};
	return res;
}

void Match::throwBall(Collision & collide)
{
	Stroke & stroke = collide.stroke;

	if (stroke.moveable.isPlayer()){
		Player & player = (Player &) stroke.moveable;
		if (player.canQuaffle()){
			PlayerQuaffle & pq = (PlayerQuaffle &) player;
			if (pq.hasQuaffle() && stroke.actionVec != Position(0, 0)){
				Displacement ballMove(_t);
				ballMove.addMove(stroke.actionVec);

				Stroke ballStroke = Stroke(_quaffle, ballMove);
				_turn.push_back(ballStroke);

				/* Put the Quaffle 1 position further in its direction */
				cout << "ACTIONVEC:" << JSON::List(stroke.actionVec) << "  "
				     << "NORMALIZED: " << JSON::List(stroke.actionVec.normalize()) << " "
				     << "frompos:" << JSON::List(collide.fromPos) << endl;
				Position qPos = collide.conflict + stroke.actionVec.normalize();
				_quaffle.setPosition(qPos);
				_pitch.insert(&_quaffle);

				JSON::Dict delta = {
					{"type", JSON::Integer(DELTA_APPEAR)},
					{"mid", JSON::Integer(player.getID())},
					{"from", JSON::List(qPos)}
				};
				_deltas.append(delta);
			}
		}
	}
}

void Match::mkSnitchStroke(void)
{
	static const Position directions[6] = {
		Pitch::West, Pitch::NorthWest, Pitch::NorthEast	,
		Pitch::East, Pitch::SouthEast, Pitch::SouthWest
	};
	if (rand()%11 < 7) /* 30% probability to move */
		return;
	Displacement move;
	for (int i=1+rand()%5; i>=0; i--){ /* Max 5 moves */
		int choosed = rand()%6;
		move.addMove(directions[choosed]);
	}
	addStroke(Stroke(_snitch, move));
}

JSON::List Match::playStrokes()
{
	/* This method process moveables moves, in regular time intervals. The
	   Moveable are moved on the pitch structure, and their positions attributes
	   are updated at the end. */

	size_t n_steps = timesteps();
	if (n_steps == 0)
		n_steps = 1;
	double tLast = 0;
	double step = 1.0/n_steps;

	/* for each interval of time in this turn do... */
	for (_t=step; _t<=1; _t+=step){
		/* for each stroke do... */
		for (Stroke & stroke : _turn){
			if (! stroke.active){
				/* The moveable with this stroke has been stopped */
				continue;
			}

			/* Current position for the moveable of this stroke */
			Position curPos  = stroke.move.position(tLast, stroke.moveable);
			Position nextPos = stroke.move.position(_t, stroke.moveable);

			cout << "Move " << stroke.moveable.getID() << " ("<<tLast<<".."<<_t<<")"<<endl;

			if (curPos == nextPos && stroke.move.length() > 0){
				/* If the moveable Position doesn't change during this
				   time interval, just skip */
				continue;
			}

			Collision collide(nextPos, curPos, stroke);

			if (! (
				stayInEllipsis(collide) ||
				keeperInZone(collide) ||
				playerCatchQuaffle(collide) ||
				seekerCatchGS(collide) ||
				scoreGoal(collide) || 
				beaterThrowBudger(collide) ||
				simpleCollision(collide)
			)){
				/* If no rule has stopped this stroke; set it to its new position */
				_pitch.setAt(nextPos, &(stroke.moveable));
				_pitch.setAt(curPos, NULL);
				cout << " &&& Regular move" << stroke.moveable.getID() << " "
					 << JSON::List(curPos) << " -> " << JSON::List(nextPos) << endl;
			}

			/* Handle throw ball strokes */
			if (stroke.action == ACT_THROW && stroke.actionPos == nextPos){
				throwBall(collide);
			}
		}
		tLast = _t;
	}

	/* Generate delta for unstopped players */
	for (Stroke & stroke : _turn){
		if (! stroke.active)
			continue;
		_deltas.append(mkDelta(stroke.moveable, stroke.move.position(1, stroke.moveable)));
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

JSON::Dict Match::getMoveables() const
{
	JSON::List squads = JSON::List();
	for (int i=0; i<2; i++)
		squads.append(JSON::Dict(_squads[i]));
	
	JSON::List balls = JSON::List();
	balls.append(JSON::Dict(_quaffle));
	balls.append(JSON::Dict(_snitch));
	for (int i=0; i<2; i++)
		balls.append(JSON::Dict(_bludgers[i]));

	JSON::Dict data = {{"balls", balls}, {"squads", squads}};
	return data;
}

/* ================== RULES ================= */
bool Match::stayInEllipsis(Collision & collide)
{
	if (_pitch.inEllipsis(collide.conflict))
		return false;

	cout << collide.stroke.moveable.getName() << " GETS OUT !!!" << endl;

	collide.stroke.active = false;
	_deltas.append(mkDelta(collide.stroke.moveable, collide.fromPos));
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
	_deltas.append(mkDelta(player, collide.fromPos));
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
	if (player.isChaser()){
		Chaser & chaser = (Chaser &) player;
		if (chaser.hasQuaffle()){
			/* If it is a chaser, and he has the ball; let him score */
			Squad *owner = id2Squad(chaser.getID());
			if (owner == &(_squads[0]) && _pitch.isInWestKeeperZone(collide.conflict)){
				_points[0] += 10;
				chaser.releaseQuaffle();
				_quaffle.setPosition(Position(0, 0));
				_pitch.insert(&_quaffle);
				JSON::Dict delta = {
					{"type", JSON::Integer(DELTA_APPEAR)},
					{"mid", JSON::Integer(chaser.getID())},
					{"from", JSON::List(_quaffle.getPosition())}
				};
				_deltas.append(delta);
				return false;
			}

			if (owner == &(_squads[1]) && _pitch.isInEastKeeperZone(collide.conflict)){
				_points[1] += 10;
				chaser.releaseQuaffle();
				_quaffle.setPosition(Position(0, 0));
				_pitch.insert(&_quaffle);
				JSON::Dict delta = {
					{"type", JSON::Integer(DELTA_APPEAR)},
					{"mid", JSON::Integer(chaser.getID())},
					{"from", JSON::List(_quaffle.getPosition())}
				};
				_deltas.append(delta);
				return false;
			}
		}
	}

	/* The Chaser try to go through its own goal; STAHP ! NOW ! */
	collide.stroke.active = false;
	_deltas.append(mkDelta(player, collide.fromPos));
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
		cout << "#" << pq.getID() << " catch the quaffle !!!" << endl;
		JSON::Dict delta = {
			{"type", JSON::Integer(DELTA_CATCH)},
			{"mid", JSON::Integer(player.getID())}
		};
		_deltas.append(delta);
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
		cout << "#" << pq.getID() << " catch the quaffle !!!" << endl;
		JSON::Dict delta = {
			{"type", JSON::Integer(DELTA_CATCH)},
			{"mid", JSON::Integer(player.getID())}
		};
		_deltas.append(delta);

		return true;
	}

	return false;
}

bool Match::simpleCollision(Collision & collide)
{

	Moveable *atPos = _pitch.getAt(collide.conflict);
	if (! atPos)
		return false; /* Nobody there, just continue */

	float firstScore  = atPos->collisionScore();
	float secondScore = collide.stroke.moveable.collisionScore();

	if (firstScore > secondScore){
		/* The first moveable on the conflicting position has a greater collision
	       score, stop the moving one. */
		collide.stroke.active = false;
		_deltas.append(mkDelta(collide.stroke.moveable, collide.fromPos));
		return true;
	}

	/* Otherwise, we eventually stop the first moveable stroke, then we swap
	   their positions */
	std::list<Stroke>::iterator it = getStrokeFor(*atPos);
	if (it != _turn.end()) (*it).active = false;
	_deltas.append(mkDelta(*atPos, collide.fromPos));
	_pitch.setAt(collide.fromPos, atPos);
	_pitch.setAt(collide.conflict, &(collide.stroke.moveable));
	return true;
}

bool Match::seekerCatchGS(Collision & collide)
{
	Moveable *atPos = _pitch.getAt(collide.conflict);
	if (! atPos)
		return false;

	/* If it is a seeker and it arrives on the GoldenSnitch */
	if (atPos->isBall()){
		Ball & ball = (Ball &) *atPos;
		if (! ball.isGoldenSnitch())
			return false;
		if (! collide.stroke.moveable.isPlayer())
			return false;
		Player & player = (Player &) collide.stroke.moveable;
		if (! player.isSeeker())
			return false;
		
		_finished = true;
		if (_squads[0].hasPlayer(&player))
			_points[0] += 150;
		else
			_points[1] += 150;
	}

	/* If it is a golden snitch that arrive on a seeker */
	if (collide.stroke.moveable.isBall()){
		Ball & ball = (Ball &) collide.stroke.moveable;
		if (! ball.isGoldenSnitch())
			return false;
		if (! atPos->isPlayer())
			return false;
		Player & player = (Player &) *atPos;
		if (! player.isSeeker())
			return false;
		
		_finished = true;
		if (_squads[0].hasPlayer(&player))
			_points[0] += 150;
		else
			_points[1] += 150;
	}
	
	return false;
}

bool Match::beaterThrowBudger(Collision & collide)
{
	Moveable *atPos = _pitch.getAt(collide.conflict);
	Stroke & stroke = collide.stroke;
	if (! atPos)
		return false;

	if (atPos->isBall()){
		Ball & ball = (Ball &) *atPos;
		if (! ball.isBludger())
			return false;
		if (! collide.stroke.moveable.isPlayer())
			return false;
		Player & player = (Player &) collide.stroke.moveable;
		if (! player.isBeater())
			return false;
		// move bluger to direction
		if (stroke.actionVec == Position(0, 0))
			return false;

		Displacement ballMove(_t);
		ballMove.addMove(stroke.actionVec);

		Stroke ballStroke = Stroke(*atPos, ballMove);
		_turn.push_back(ballStroke);

		Position qPos = collide.conflict + stroke.actionVec.normalize();

		_deltas.append(mkDelta(*atPos, qPos));
		atPos->setPosition(qPos);
		_pitch.setAt(collide.conflict, &player);
		_pitch.setAt(collide.fromPos, NULL);
		return true;
	}
	
	return false;

}