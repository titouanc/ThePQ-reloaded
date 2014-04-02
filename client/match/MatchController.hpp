#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER

#include <ClientController.hpp>
#include <model/Pitch.hpp>
#include <model/Squad.hpp>
#include <model/Ball.hpp>
#include <model/Displacement.hpp>
#include <model/Stroke.hpp>

class MatchController : public ClientController {
	public:
		typedef enum {CREATED, READY, PROMPT, TIMEOUT, FINISHED} State;
				
	private:
		Pitch _pitch;
		Squad _mySquad, _otherSquad;
		State _state;
		Quaffle *_quaffle;

		void treatBalls(JSON::List const & balls);
		void treatSquads(JSON::List const & squads);
		void treatDeltas(JSON::List const & delta);

	protected:
		/* logic-related methods */
		Squad & mySquad() {return _mySquad;}
		Pitch & pitch() {return _pitch;}
		bool isMyPlayer(Player const & player) const;
		State state() const {return _state;}

		/* Send a player displacement to the server */
		void sendDisplacement(Player const & player, Displacement const & move);

		/* Send a player displacement to the server */
		void sendStroke(Stroke const & stroke);

		/* HOOKS */
		/* Called after pitch gets updated */
		virtual void onPitchChange(){}
		/* Called when the Squads are initialized at the beginning. */
		virtual void onSquadsInitialized(){}
		/* Called after state gets updated */
		virtual void onStateChange(){}
		/* Called when there is an error */
		virtual void onError(std::string const & info){}
		/* Treat Match specific messages */
		void treatMessage(std::string const & type, JSON::Value const * msg);

	public:
		using ClientController::ClientController;
		MatchController(ClientController const & other);
		virtual ~MatchController();

};

#endif
