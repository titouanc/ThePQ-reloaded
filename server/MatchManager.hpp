#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <cassert>

#include <sys/select.h>
#include <deque>
#include <model/Moveable.hpp>
#include <model/Displacement.hpp>
#include <model/Pitch.hpp>
#include <model/Player.hpp>
#include <model/Ball.hpp>
#include <cstring>
#include <model/User.hpp>
#include <network/ConnectionManager.hpp>
#include <model/Player.hpp>
#include <model/Squad.hpp>
#include <model/MatchResult.hpp>
#include <Constants.hpp>
#include <network/SubConnectionManager.hpp>
#include <utility>

#include "Match.hpp"

using namespace net;

class MatchManager : public SubConnectionManager {
	private:
		Match _match;
		SharedQueue<Message> _inbox, _outbox;
		bool _champMatch;
		MatchResult _matchRes;
		
		/* Processing of incoming messages*/
		void processMessage(Message const & msg);
		/* Process incoming message with given data (a JSON::Dict)
		   Could not be made by Stroke(JSON::Dict) because we have to map
		   moveable_id -> Moveable */
		void processStroke(Message const & msg, JSON::Dict const & data);
		/* Create random moves for golden snitch */
		//void mkSnitchStroke(void);

		/* Send messages to clients */
		void sendToAll(JSON::Value const & data);

		/* Send {"type": "<signal>", "data": true} to everyone */
		void sendSignal(std::string const & sig);

		/* Send message to <msg> sender with given <type> and payload */
		void reply(Message const & msg, std::string type, JSON::Value const & data);

		/* Like ^ but data set to "<text>" */
		void reply(Message const & msg, std::string type, const char *text);

		/* Send squads composition to everyone */
		void sendMoveables(void);

		void resolveFame(std::string win,std::string los);
		void resolveFameDisconnection(std::string win);
		void resolveMoneyDisconnection(std::string win);
		void resolveMoney(std::string win,std::string los);
		
		void endMatch(void);
	public:
		MatchManager(
			BaseConnectionManager & connections, 
			Squad const & squadA, Squad const & squadB,
			bool champMatch
		);
		virtual ~MatchManager();
		/* Run dat shit */
		void _mainloop_out();
		bool isChampMatch() const;
		MatchResult getResult() const;
		std::pair<std::string,unsigned int> getWinner() const {return _match.getWinner();}
		std::pair<std::string,unsigned int> getLoser() const {return _match.getLoser();}
};

#endif
