#include "MatchManager.hpp"

/* TODO: read from config file */
#define STROKES_TIMEOUT_SECONDS 3

MatchManager::MatchManager(
	BaseConnectionManager & connections, 
	Squad const & squadA, Squad const & squadB
) : SubConnectionManager(_inbox, _outbox, connections), 
    _strokes(), _pitch(100, 36)
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
	acquireClient(squadA.client_id);
	acquireClient(squadB.client_id);
	initPositions();
	cout << "[" << this << "] \033[1m\033[32mMatch created\033[0m" << endl;
}

void MatchManager::initPositions(void)
{
	Position const & c = _pitch.center();
	for (int i=0; i<2; i++){
		int k = i ? 1 : -1;
		_squads[i].chasers[0].setPosition(c + k*Pitch::West);
		_squads[i].chasers[1].setPosition(c + 2*k*Pitch::NorthWest);
		_squads[i].chasers[2].setPosition(c + 2*k*Pitch::SouthWest);
		_squads[i].beaters[0].setPosition(c + k*Pitch::West + k*Pitch::SouthWest);
		_squads[i].beaters[1].setPosition(c + k*Pitch::West + k*Pitch::NorthWest);
		_squads[i].seeker.setPosition(c + 2*k*Pitch::West);
		_squads[i].keeper.setPosition(c + 10*k*Pitch::West);
		for (int j=0; j<7; j++)
			_pitch.insert(_squads[i].players[j]);
	}
}

MatchManager::~MatchManager()
{
	while (_outbox.available());
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
	
	cout << "Got displacement: " << move.toJson() << endl;

	_strokes.push(Stroke(player, move));
	reply(msg, MATCH_ACK, data);
	cout << _strokes.back().move.toJson() << endl;
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
	sendSquads();
	time_t tick;
	cout << "[" << this << "] \033[32mMatch started\033[0m" << endl;
	sendSignal(MATCH_START);

	while (nClients() > 0){
		cout << "+ tick match #" << this << endl;
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

/* send squads composition */
void MatchManager::sendSquads(void)
{
	int i;
	JSON::List *list = new JSON::List();
	for (i=0; i<2; i++)
		list->append(JSON::Dict(_squads[i]));
	
	JSON::Dict msg;
	msg.setPtr("data", list);
	msg.set("type", MATCH_SQUADS);
	sendToAll(msg);
}

void MatchManager::sendMatchDeltas(JSON::List const & delta)
{
	JSON::Dict msg;
	msg.set("type", MATCH_DELTA);
	msg.set("data", delta);
	sendToAll(msg);
}

Stroke MatchManager::getStrokeForMoveable(Moveable *moveable)
{
	size_t n_strokes = _strokes.size();
	for (size_t i=0; i<n_strokes; i++){
		Stroke s = _strokes.front();
		_strokes.pop();
		if (&(_strokes.front().moveable) == moveable)
			return s;
		_strokes.push(s);
	}
	throw std::runtime_error("No stroke for this moveable !");
}

/*!
 * @meth void MatchManager::playStrokes(void)
 * @brief Play all strokes in this turn
 */
void MatchManager::playStrokes(void)
{
	size_t i, len, maxlen=0;
	JSON::List finalPositions;

	/* Find longest displacement */
	for (i=0; i<_strokes.size(); i++){
		Stroke stroke = _strokes.front();
		_strokes.pop();
		_strokes.push(stroke);
		len = stroke.move.length();
		if (len > maxlen)
			maxlen = len;
	}

	/* For each step in time interval... */
	double last_t = 0;
	for (double t=1.0/maxlen; t<=1; t+=1.0/maxlen){
		size_t n_strokes = _strokes.size();
		/* For each stroke */
		for (i=0; i<n_strokes; i++){
			Stroke stroke = _strokes.front();
			_strokes.pop();
			
			/* Where the moveable would be */
			Position newPos = stroke.move.position(t, stroke.moveable);
			Position oldPos = stroke.move.position(last_t, stroke.moveable);

			/* Who's there ? */
			Moveable *atPos = _pitch.getAt(newPos);
			cout << stroke.moveable.getName() << " " 
			     << oldPos.toJson()
			     << " -> " << newPos.toJson() << endl;

			if (atPos){
				switch (onCollision(*atPos, stroke.moveable, newPos)){
					case FIRST_WIN: {
						/* First on position wins => stop the second moveable */
						stroke.moveable.setPosition(oldPos);
						JSON::Dict serialized;
						serialized.set("mid", stroke.moveable.getID());
						serialized.set("finalpos", oldPos.toJson());
						finalPositions.append(serialized);
						break;
					}
					case SECOND_WIN:
						cout << "Second moveable wins" << endl;
						break;
					default:
						cout << "Nothing todo" << endl;
						break;
				}
			} else {
				_pitch.setAt(newPos, &(stroke.moveable));
				_pitch.setAt(oldPos, NULL);
				_strokes.push(stroke);
			}
		}
		last_t = t;
	}



	while (! _strokes.empty()){
		Stroke & stroke = _strokes.front();
		Position const & pos = stroke.move.position(1, stroke.moveable);

		JSON::Dict serialized;
		serialized.set("mid", stroke.moveable.getID());
		serialized.set("finalpos", pos.toJson());
		finalPositions.append(serialized);
		_strokes.pop();
	}

	sendMatchDeltas(finalPositions);
}


MatchManager::Collision MatchManager::onCollision(
	Moveable & first, 
	Moveable & second,
	Position & conflict
)
{
	float first_score  = first.collisionScore();
	float second_score = second.collisionScore();

	cout << "Collision. " << first.getName() << "(" << first_score << ") on " 
	     << conflict.toJson() << " and " << second.getName() << "(" 
	     << second_score << ") arrives." << endl;

	if (first_score > second_score)
		return FIRST_WIN;
	else 
		return SECOND_WIN;
}
