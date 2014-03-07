#include "MatchManager.hpp"
#include <Constants.hpp>
#include <model/Ball.hpp>

using namespace net;

void MatchManager::treatBalls(JSON::List const & balls)
{
	Quaffle *q = new Quaffle(DICT(balls[0]));
	_pitch.insert(q);
	GoldenSnitch *g = new GoldenSnitch(DICT(balls[1]));
	_pitch.insert(g);
	for (size_t i=2; i<4; i++){
		Bludger *b = new Bludger(DICT(balls[i]));
		_pitch.insert(b);
	}
	onPitchChange();
}

void MatchManager::treatSquads(JSON::List const & squads)
{
	_mySquad = DICT(squads[0]);
	_otherSquad = DICT(squads[1]);
	for (int i=0; i<7; i++){
		_pitch.insert(_mySquad.players[i]);
		if (_mySquad.players[i]->getSpeed() == 0)
			_mySquad.players[i]->setSpeed(5);
		_pitch.insert(_otherSquad.players[i]);
		if (_otherSquad.players[i]->getSpeed() == 0)
			_otherSquad.players[i]->setSpeed(5);
	}
	onPitchChange();
}

void MatchManager::treatDeltas(JSON::List const & deltas)
{
	for (size_t i=0; i<deltas.len(); i++){
		JSON::Dict const & delta = DICT(deltas[i]);
		Position from(LIST(delta.get("from")));
		Position to(LIST(delta.get("to")));

		Moveable *toMove = _pitch.getAt(from);
		_pitch.setAt(to, toMove);
		_pitch.setAt(from, NULL);
		toMove->setPosition(to);
		cout << "MOVE " << toMove->getName() << " " 
		     << JSON::List(from) << " -> " << JSON::List(to) << endl;
	}
	onPitchChange();
}

MatchManager::MatchManager(ClientManager const & other) : ClientManager(other), _state(CREATED)
{}

MatchManager::~MatchManager()
{
	for (Pitch::iterator it=_pitch.begin(); it!=_pitch.end();){
		Pitch::iterator next = it;
		next++;
		if (it->second->isBall())
			delete it->second;
		_pitch.setAt(it->first, NULL);
		it = next;
	}
}

void MatchManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	cout << "TREAT MESSAGE " << type << endl;

	if (type == MSG::MATCH_PROMPT){
		_state = PROMPT;
		onStateChange();
	} else if (type == MSG::MATCH_TIMEOUT){
		_state = TIMEOUT;
		onStateChange();
	} else if (type == MSG::MATCH_END){
		_state = FINISHED;
		onStateChange();
	}

	else if (type == MSG::MATCH_BALLS)
		treatBalls(LIST(data));
	else if (type == MSG::MATCH_SQUADS)
		treatSquads(LIST(data));

	else if (type == MSG::MATCH_DELTA)
		treatDeltas(LIST(data));
}

void MatchManager::sendDisplacement(Player const & player, Displacement const & move)
{
	JSON::Dict msg = {
		{"mid", JSON::Integer(player.getID())},
		{"move", JSON::List(move)}
	};
	say(MSG::MATCH_STROKE, msg);
}

bool MatchManager::isMyPlayer(Player const & player) const
{
	for (int i=0; i<7; i++)
		if (_mySquad.players[i] == &player)
			return true;
	return false;
}
