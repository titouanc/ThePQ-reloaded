#include "MatchManager.hpp"
#include <toolbox.hpp>
#include <ctime>
#include <cstdlib>

/* TODO: read from config file */
#define STROKES_TIMEOUT_SECONDS 10

MatchManager::MatchManager(
	BaseConnectionManager & connections, 
	Squad const & squadA, Squad const & squadB,
	bool champMatch
) : 
	SubConnectionManager(_inbox, _outbox, connections), 
	_match(squadA, squadB),
	_champMatch(champMatch)
{
	/* Get a local communication channel */
	acquireClient(squadA.client_id);
	acquireClient(squadB.client_id);
	cout << "[" << this << "] \033[1m\033[32mMatch created\033[0m" << endl;
}

MatchManager::~MatchManager()
{
	cout << "[" << this << "] \033[1m\033[32mMatch destroyed\033[0m" << endl;
}

void MatchManager::processStroke(Message const & msg, JSON::Dict const & data)
{
	if (_match.addStroke(data))
		reply(msg, net::MSG::MATCH_ACK, data);
}

void MatchManager::processMessage(Message const & msg)
{
	JSON::Dict const & data = DICT(msg.data);
	if (! ISSTR(data.get("type")))
		return reply(msg, net::MSG::MATCH_ERROR, "Missing \"type\":string");
	if (STR(data.get("type")).value() == net::MSG::MATCH_STROKE){
		if (! ISDICT(data.get("data")))
			return reply(msg, net::MSG::MATCH_ERROR, "Data should be a dict");
		processStroke(msg, DICT(data.get("data")));
	}
}

void MatchManager::_mainloop_out()
{
	sendMoveables();
	time_t tick;
	cout << "[" << this << "] \033[32mMatch started\033[0m" << endl;
	sendSignal(net::MSG::MATCH_START);

	unsigned int n_ticks = 0;
	while (nClients() == 2 && ! _match.isFinished()){
		n_ticks++;
		cout << "[" << this << "] + tick " << n_ticks << endl;
		sendSignal(net::MSG::MATCH_PROMPT);
		tick = time(NULL);

		do {
			if (_inbox.available()){
				Message const & msg = _inbox.pop();
				if (ISDICT(msg.data))
					processMessage(msg);
				else
					reply(msg, net::MSG::MATCH_ERROR, "Not a dict");
				delete msg.data;
			} else {
				minisleep(0.1);
			}
		} while (time(NULL) - tick < STROKES_TIMEOUT_SECONDS);
		
		sendSignal(net::MSG::MATCH_TIMEOUT);

		JSON::List const & deltas = _match.playStrokes();
		JSON::Dict msg = {
			{"type", JSON::String(net::MSG::MATCH_DELTA)},
			{"data", deltas}
		};
		sendToAll(msg);
	}

	if (hasClient(_match.squad(0).client_id)){
		_matchRes.setScore(100,0);
		_matchRes.setTeams(_match.squad(0).squad_owner,_match.squad(1).squad_owner);
		_matchRes.resolveFameDisconnection(_match.squad(0).squad_owner);
		_matchRes.resolveMoneyDisconnection(_match.squad(0).squad_owner);
	}

	if (hasClient(_match.squad(1).client_id)){
		_matchRes.setScore(100,0);
		_matchRes.setTeams(_match.squad(1).squad_owner,_match.squad(0).squad_owner);
		_matchRes.resolveFameDisconnection(_match.squad(1).squad_owner);
		_matchRes.resolveMoneyDisconnection(_match.squad(1).squad_owner);
	}

	sendSignal(net::MSG::MATCH_END);
	cout << "[" << this << "] \033[32mMatch finished\033[0m" << endl;
	stop();
	for (int i=0; i<2; i++)
		releaseClient(_match.squad(i).client_id);
}

bool MatchManager::isChampMatch() const {
	return _champMatch;
}

MatchResult MatchManager::getResult() const {
	return _matchRes;
}

void MatchManager::sendToAll(JSON::Value const & data)
{
	for (int i=0; i<2; i++)
		_doWrite(_match.squad(i).client_id, &data);
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
	response.set("type", net::MSG::MATCH_STATUS);
	JSON::Dict status = JSON::Dict();
	status.set("type", type);
	status.set("data", data);
	response.set("data", status);
	_doWrite(msg.peer_id, &response);
}

void MatchManager::reply(Message const & msg, std::string type, const char *text)
{
	reply(msg, type, JSON::String(text));
}

void MatchManager::sendMoveables(void)
{
	JSON::Dict msg = {
		{"type", JSON::String(MSG::MATCH_MOVEABLES)}, 
		{"data", _match.getMoveables()}
	};
	sendToAll(msg);
}

void MatchManager::endMatch(void)
{
	/*
	int winner = (_score[0] > _score[1]) ? 0 : 1;
	int looser = 1-winner;
	
	JSON::Dict winnerScore;
	winnerScore.set("squad_id", _match.squad(winner).squad_id);
	winnerScore.set("score", _score[winner]);

	JSON::Dict looserScore;
	looserScore.set("squad_id", _match.squad(looser).squad_id);
	looserScore.set("score", _score[looser]);

	JSON::Dict data;
	data.set("winner", winnerScore);
	data.set("looser", looserScore);
	
	JSON::Dict msg;
	msg.set("type", net::MSG::MATCH_SCORES);
	msg.set("data", data);
	sendToAll(msg);
	sendSignal(net::MSG::MATCH_END);

	releaseClient(_match.squad(0).client_id);
	releaseClient(_match.squad(1).client_id);

	_matchRes.setTeams(_match.squad(winner).squad_owner,_match.squad(looser).squad_owner);
	_matchRes.setScore(_score[winner], _score[looser]);
	*/
}

