#include "MatchManager.hpp"
#include <Constants.hpp>
#include <model/Ball.hpp>
#include <model/Stroke.hpp>

using namespace net;


/**
 * Method handling ball placement on the pitch
 * @param JSON::List : list of all the balls for a game
 */
void MatchManager::treatBalls(JSON::List const & balls)
{
	Quaffle *q = new Quaffle(DICT(balls[0]));
	_pitch.insert(q);
	_quaffle = q;
	GoldenSnitch *g = new GoldenSnitch(DICT(balls[1]));
	_pitch.insert(g);
	for (size_t i=2; i<4; i++){
		Bludger *b = new Bludger(DICT(balls[i]));
		_pitch.insert(b);
	}
	onPitchChange();
}

/**
 * Method handling squad placement on the pitch depending on the user
 * @param JSON::List : list of the squads 
 */
void MatchManager::treatSquads(JSON::List const & squads)
{
	Squad sq1(DICT(squads[0]));
	Squad sq2(DICT(squads[1]));

	if (sq1.squad_owner == user().username){
		_mySquad = sq1;
		_otherSquad = sq2;
	} else {
		_mySquad = sq2;
		_otherSquad = sq1;
	}

	for (int i=0; i<7; i++){
		_pitch.insert(_mySquad.players[i]);
		if (_mySquad.players[i]->getSpeed() == 0)
			_mySquad.players[i]->setSpeed(5);
		_pitch.insert(_otherSquad.players[i]);
		if (_otherSquad.players[i]->getSpeed() == 0)
			_otherSquad.players[i]->setSpeed(5);
	}
	onSquadsInitialized();
	onPitchChange();
}

/**
 * Method handling movements on the pitch
 * @param JSON::List : list of all the movements to be treated 
 */
void MatchManager::treatDeltas(JSON::List const & deltas)
{
	for (size_t i=0; i<deltas.len(); i++){
		JSON::Dict const & delta = DICT(deltas[i]);
		DeltaType type = (DeltaType) INT(delta.get("type")).value();

		if (type == DELTA_MOVE){
			Position from(LIST(delta.get("from")));
			Position to(LIST(delta.get("to")));

			if (from != to){
				Moveable *toMove = _pitch.getAt(from);
				_pitch.setAt(to, toMove);
				_pitch.setAt(from, NULL);
				toMove->setPosition(to);
				cout << "MOVE " << toMove->getName() << " " 
			    	 << JSON::List(from) << " -> " << JSON::List(to) << endl;
			}
		} 

		/* Quaffle throwed; appear on pitch */
		else if (type == DELTA_APPEAR){
			Position pos(LIST(delta.get("from")));
			_pitch.setAt(pos, _quaffle);
			_quaffle->setPosition(pos);

			for (std::pair<const Position,Moveable*> & it : _pitch){
				if (it.second->getID() == INT(delta.get("mid"))){
					PlayerQuaffle *pq = (PlayerQuaffle *) it.second;
					pq->releaseQuaffle();
					cout << pq->getName() << " THROW QUAFFLE" << endl;
					break;
				}
			}
		} 

		/* Quaffle catched; disappear from pitch */
		else if (type == DELTA_CATCH){
			for (std::pair<const Position,Moveable*> & it : _pitch){
				if (it.second->getID() == INT(delta.get("mid"))){
					PlayerQuaffle *pq = (PlayerQuaffle *) it.second;
					pq->retainQuaffle();
					cout << pq->getName() << " CATCH QUAFFLE" << endl;
				}
			}
			_pitch.setAt(_quaffle->getPosition(), NULL);
		}
	}
	onPitchChange();
}
///Constructor
MatchManager::MatchManager(ClientController const & other) 
: ClientController(other), _state(CREATED), _quaffle(NULL)
{}

///Destructor
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

/**
 * Method handling server queries
 * @param string : type of the query
 * @param JSON::Value : queries to be treated
 */
void MatchManager::treatMessage(std::string const & type, JSON::Value const * data)
{
	cout << "TREAT MESSAGE " << type << endl;

	/* unwrap MSTATUS messages */
	if (type == MSG::MATCH_STATUS){
		/* TODO: remove MSTATUS wrapper message !!! */
		std::string const & real_type = STR(DICT(data).get("type"));
		treatMessage(real_type, DICT(data).get("data"));
	}

	else if (type == MSG::MATCH_PROMPT){
		_state = PROMPT;
		onStateChange();
	} else if (type == MSG::MATCH_TIMEOUT){
		_state = TIMEOUT;
		onStateChange();
	} else if (type == MSG::MATCH_END){
		_state = FINISHED;
		onStateChange();
	}

	else if (type == MSG::MATCH_MOVEABLES){
		treatBalls(LIST(DICT(data).get("balls")));
		treatSquads(LIST(DICT(data).get("squads")));
		if (_state == CREATED){
			_state = READY;
			onStateChange();
		}
	}

	else if (type == MSG::MATCH_DELTA)
		treatDeltas(LIST(data));

	else if (type == MSG::MATCH_ERROR)
		onError(STR(data));
}

/**
 * Method
 */
void MatchManager::sendStroke(Stroke const & stroke)
{
	say(MSG::MATCH_STROKE, JSON::Dict(stroke));
}

/**
 * Method handlind queries to the server
 * @param Player : target of the query
 * @param Displacement : query to send
 */
void MatchManager::sendDisplacement(Player const & player, Displacement const & move)
{
	JSON::Dict msg = {
		{"mid", JSON::Integer(player.getID())},
		{"move", JSON::List(move)}
	};
	say(MSG::MATCH_STROKE, msg);
}

/**
 * Method handling player ownership
 * @param Player : player whose ownership is checked
 * @return bool : ownership status of player
 */
bool MatchManager::isMyPlayer(Player const & player) const
{
	for (int i=0; i<7; i++)
		if (_mySquad.players[i] == &player)
			return true;
	return false;
}
