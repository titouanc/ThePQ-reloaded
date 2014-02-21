#include "MatchManager.hpp"

/* TODO: read from config file */
#define STROKES_TIMEOUT_SECONDS 3

MatchManager::MatchManager(
	BaseConnectionManager & connections, 
	Squad const & squadA, Squad const & squadB
) : SubConnectionManager(_inbox, _outbox, connections), 
    _strokes(), _pitch(100, 36)
{
	_score[0] = 0;
	_score[1] = 0;

	_quaffle.setID(101);
	_snitch.setID(102);
	_bludgers[0].setID(103);
	_bludgers[1].setID(104);

	_squads[0] = squadA;
	_squads[1] = squadB;

	for (int i=0; i<2; i++){
		for (int j=0; j<7; j++){
			/* Players IDs: 1..7 and 11..16*/
			_squads[i].players[j]->setID(10*i+j+1);
		}
	}

	/* Get a local communication channel */
	acquireClient(squadA.client_id);
	acquireClient(squadB.client_id);
	initPositions();
	cout << "[" << this << "] \033[1m\033[32mMatch created\033[0m" << endl;
}

void MatchManager::initPositions(void)
{
	Position const & c = _pitch.center();
	_quaffle.setPosition(c);
	_pitch.insert(&_quaffle);
	_snitch.setPosition(c + 6*Pitch::SouthWest);
	_pitch.insert(&_snitch);

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
}

MatchManager::~MatchManager()
{
	cout << "[" << this << "] \033[1m\033[32mMatch destroyed\033[0m" << endl;
}

void MatchManager::minisleep(double secs)
{
	assert(secs >= 0);
	int seconds = secs;
	int micros  = secs*1000000 - seconds;

	struct timeval interval = {seconds, micros};
	select(1, NULL, NULL, NULL, &interval);
}

void MatchManager::processStroke(Message const & msg, JSON::Dict const & data)
{
	if (! ISINT(data.get("mid")))
		return reply(msg, MATCH_ERROR, "Missing moveable ID");
	int mid = INT(data.get("mid"));
	if (mid < 1 || (mid < 11 && mid > 7) || mid > 17)
		return reply(msg, MATCH_ERROR, "Not a player");
	int squad_i = mid/10;
	int player_i = mid%10 - 1;

	if (msg.peer_id != _squads[squad_i].client_id)
		return reply(msg, MATCH_ERROR, "Not your player");

	if (! ISLIST(data.get("move")))
		return reply(msg, MATCH_ERROR, "No displacement found");

	Displacement move(LIST(data.get("move")));
	Moveable & player = *(_squads[squad_i].players[player_i]);
	
	_strokes.push_back(Stroke(player, move));
	reply(msg, MATCH_ACK, data);
}

void MatchManager::processMessage(Message const & msg)
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

void MatchManager::_mainloop_out()
{
	sendBalls();
	sendSquads();
	time_t tick;
	cout << "[" << this << "] \033[32mMatch started\033[0m" << endl;
	sendSignal(MATCH_START);

	cout << _pitch << endl;

	unsigned int n_ticks = 0;
	while (nClients() > 0){
		n_ticks++;
		cout << "[" << this << "] + tick " << n_ticks << endl;
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
		} while (time(NULL) - tick < STROKES_TIMEOUT_SECONDS);
		
		sendSignal(MATCH_TIMEOUT);
		if (! _strokes.empty())
			playStrokes();

		cout << _pitch << endl;

		if (n_ticks > 3)
			break;
	}

	sendSignal(MATCH_END);
	cout << "[" << this << "] \033[32mMatch finished\033[0m" << endl;
	stop();
	for (int i=0; i<2; i++)
		releaseClient(_squads[i].client_id);
}

void MatchManager::sendToAll(JSON::Value const & data)
{
	for (int i=0; i<2; i++)
		_doWrite(_squads[i].client_id, &data);
}

void MatchManager::sendSignal(std::string const & sig)
{
	JSON::Dict msg;
	msg.set("type", sig);
	msg.set("data", JSON::Bool(true));
	sendToAll(msg);
}

void MatchManager::reply(Message const & msg, std::string type, JSON::Value const & data)
{
	JSON::Dict response = JSON::Dict();
	response.set("type", type);
	response.set("data", data);
	_doWrite(msg.peer_id, &response);
}

void MatchManager::reply(Message const & msg, std::string type, const char *text)
{
	reply(msg, type, JSON::String(text));
}

void MatchManager::mkSnitchStroke(void)
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
	_strokes.push_back(Stroke(_snitch, move));
}

/* send squads composition */
void MatchManager::sendSquads(void)
{
	JSON::List list = JSON::List();
	for (int i=0; i<2; i++)
		list.append(JSON::Dict(_squads[i]));
	
	JSON::Dict msg;
	msg.set("data", list);
	msg.set("type", MATCH_SQUADS);
	sendToAll(msg);
}

void MatchManager::sendBalls(void)
{
	JSON::List balls = JSON::List();
	balls.append(JSON::Dict(_quaffle));
	balls.append(JSON::Dict(_snitch));
	for (int i=0; i<2; i++)
		balls.append(JSON::Dict(_bludgers[i]));
	
	JSON::Dict msg;
	msg.set("data", balls);
	msg.set("type", MATCH_BALLS);
	sendToAll(msg);
}

void MatchManager::sendMatchDeltas(void)
{
	JSON::Dict msg;
	msg.set("type", MATCH_DELTA);
	msg.set("data", _turnDeltas);
	sendToAll(msg);
}

MatchManager::iter MatchManager::getStrokeForMoveable(Moveable *moveable)
{
	iter res;
	for (res=_strokes.begin(); res!=_strokes.end(); res++){
		if (&((*res).moveable) == moveable)
			break;
	}
	return res;
}

void MatchManager::addDelta(Moveable const & moveable, Position const & dest)
{
	JSON::Dict res;
	res.set("mid", moveable.getID());
	res.set("from", moveable.getPosition().toJson());
	res.set("to", dest.toJson());
	_turnDeltas.append(res);
}

void MatchManager::stopStroke(Stroke & stroke, Position const & pos)
{
	stroke.active = false;
	if (stroke.moveable.getPosition() != pos)
		addDelta(stroke.moveable, pos);
	stroke.moveable.setPosition(pos);
}

bool MatchManager::checkGoal(
	Stroke & stroke,     /* Stroke that might lead to goal */
	Position & toPos,    /* Position that might be a goal */
	Position & fromPos   /* last pos occupied by moving */
)
{
	Moveable & moving = stroke.moveable;
	bool isWestGoal = _pitch.isWestGoal(toPos.x(), toPos.y());
	bool isEastGoal = _pitch.isEastGoal(toPos.x(), toPos.y());
	if (! (isWestGoal || isEastGoal))
		return false;

	if (moving.isBall()){
		Ball & ball = (Ball &) moving;
		if (ball.isQuaffle()){
			/* If quaffle GOOOOOLEGOOLEGOOOOLE !!!*/
			if (isWestGoal)
				_score[0] += 10;
			else
				_score[1] += 10;
			return true;
		}
	}

	/* Otherwise stop moveable */
	stopStroke(stroke, fromPos);
	return false;
}

void MatchManager::playStrokes(void)
{
	size_t len, maxlen=0;
	/* Find longest displacement */
	for (iter i=_strokes.begin(); i!=_strokes.end(); i++){
		len = (*i).move.length();
		if (len > maxlen)
			maxlen = len;
	}

	JSON::List turnDeltas;
	double last_t = 0;
	for (size_t t_i=1; t_i<=maxlen; t_i++){
		/* Foreach time step */
		_t = ((double) t_i)/maxlen;
		for (iter i=_strokes.begin(); i!=_strokes.end(); i++){
			/* Foreach active stroke */
			if (! (*i).active)
				continue;

			Moveable & moving = (*i).moveable;
			Displacement & move = (*i).move;
			Position newPos = move.position(_t, moving);
			Position oldPos = move.position(last_t, moving);
			if (newPos == oldPos)
				continue;

			if (checkGoal(*i, newPos, oldPos)){
				/* Is it a goal ? */
				cout << "GOOOOOLEGOOLEGOOOOLE" << endl;
				continue;
			}
			
			if (! _pitch.inEllipsis(newPos)){
				/* Stop the moveable before it gets out */
				stopStroke(*i, oldPos);
				continue;
			}

			if (moving.isPlayer()){
				/* Don't let the keeper get out of his zone */
				Player & player = (Player &) moving;
				if (player.isKeeper() && ! _pitch.isInKeeperZone(newPos)){
					stopStroke(*i, oldPos);
					continue;
				}
			}

			/* If there is someone where we would arrive */
			Moveable *atNewPos = _pitch.getAt(newPos);

			if (atNewPos != NULL && atNewPos != &moving){
				onCollision(*i, newPos, oldPos);
			} else {
				/* Move to newly acquired position */
				_pitch.setAt(oldPos, NULL);
				_pitch.setAt(newPos, &moving);
			}
		}
		last_t = _t;
	}

	/* Find all final position of unstopped moveable */
	for (iter i=_strokes.begin(); i!=_strokes.end(); i++){
		if ((*i).active){
			Moveable & moving = (*i).moveable;
			Displacement & move = (*i).move;
			Position finalPos = move.position(1, moving);
			addDelta(moving, finalPos);
			moving.setPosition(finalPos);
		}
	}

	/* Tell clients about modifications */
	sendMatchDeltas();
	/* Reset turn state */
	_turnDeltas = JSON::List();
	_strokes.clear();
	mkSnitchStroke();
}

void MatchManager::throwBall(
	Moveable & ball,
	Position & fromPos,
	Position & direction
)
{
	iter stoppedStroke = getStrokeForMoveable(&ball);
	if (stoppedStroke != _strokes.end()){
		/* If the ball was moving, remove it's stroke */
		(*stoppedStroke).active = false;
		JSON::Dict delta;
		delta.set("mid", ball.getID());
		delta.set("from", ball.getPosition().toJson());
		delta.set("to", fromPos.toJson());
		_turnDeltas.append(delta);
		ball.setPosition(fromPos);
		_pitch.setAt(fromPos, &ball);
	}

	Displacement newMove(_t); /* Start a new move at this timestep */
	newMove.addMove(direction);
	_strokes.push_back(Stroke(ball, newMove));
}

void MatchManager::endMatch(void)
{
	sendMatchDeltas();

	int winner = (_score[0] > _score[1]) ? 0 : 1;
	int looser = 1-winner;
	
	JSON::Dict winnerScore;
	winnerScore.set("squad_id", _squads[winner].squad_id);
	winnerScore.set("score", _score[winner]);

	JSON::Dict looserScore;
	looserScore.set("squad_id", _squads[looser].squad_id);
	looserScore.set("score", _score[looser]);

	JSON::Dict data;
	data.set("winner", winnerScore);
	data.set("looser", looserScore);
	
	JSON::Dict msg;
	msg.set("type", MATCH_SCORES);
	msg.set("data", data);
	sendToAll(msg);
	sendSignal(MATCH_END);

	releaseClient(_squads[0].client_id);
	releaseClient(_squads[1].client_id);
}

void MatchManager::onCollision(
	Stroke & stroke,     /* Stroke that leads to conflict */
	Position & conflict, /* Clonflicting pos */
	Position & fromPos  /* last pos occupied by moving */
)
{
	Moveable & first  = *(_pitch.getAt(conflict));
	Moveable & moving = stroke.moveable;
	float first_score  = first.collisionScore();
	float second_score = moving.collisionScore();

	if (first.isBall() && moving.isPlayer()){
		Player & player = (Player &) moving;
		Ball & ball = (Ball &) first;

		if (ball.isGoldenSnitch() && player.isSeeker()){
			if (&player == &(_squads[0].seeker)){
				_score[0] += 150;
			} else {
				_score[1] += 150;
			}
			stopStroke(stroke, conflict);
			iter stoppedStroke = getStrokeForMoveable(&ball);
			if (stoppedStroke != _strokes.end()){
				stopStroke(*stoppedStroke, conflict);
			}
			endMatch();
			return;
		}

		/* Chasers can throw quaffle; Beaters can bat bludgers
		   But... LATER !!! */
		/*
		if ((ball.isQuaffle() && player.isChaser()) || 
			(ball.isBludger() && player.isBeater())){
			Position newDirection = conflict - fromPos;
			throwBall(ball, conflict, newDirection);
			_pitch.setAt(fromPos, NULL);
			_pitch.setAt(conflict, &moving);
		}*/
	}
	if (first_score > second_score){

		/* The first moveable wins, second stops on fromPos */
		stopStroke(stroke, fromPos);
	} else {

		/* Second wins: stop first on fromPos */
		_pitch.setAt(fromPos, &first);
		_pitch.setAt(conflict, &moving);
		iter stoppedStroke = getStrokeForMoveable(&first);
		if (stoppedStroke != _strokes.end()){
			stopStroke(*stoppedStroke, fromPos);
		}
	}
}
