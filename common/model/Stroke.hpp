#ifndef DEFINE_STROKE_HEADER
#define DEFINE_STROKE_HEADER

#include <model/Displacement.hpp>
#include <model/Moveable.hpp>

/*! Type of action in a stroke */
enum ActionType {
	ACT_NONE=0,  //!< No action
	ACT_BAT=1,   //!< Bat action
	ACT_THROW=2  //!< Throw quaffle action
};

/*! Type of a MatchDelta */
enum DeltaType {
	DELTA_MOVE=1,  //!< Regular delta (move)
	DELTA_CATCH=2, //!< Quaffle catched 
	DELTA_APPEAR=3 //!< Ball reappear (was throwed)
};

struct Stroke {
	Moveable & moveable;

	/*! Displacement for the moveable */
	Displacement move;

	/*! Is this stroke active (could be deactivated after a collision) */
	bool active;

	/* Action that could happen */
	ActionType action;

	/*! Position where action could happen (if any) */
	Position actionPos;

	/*! Direction for the action, if any */
	Position actionVec;
	
	/*!
	 */
	Stroke(
		Moveable & mov, 
		Displacement const & disp=Displacement(), 
		ActionType act=ACT_NONE,
		Position const & act_at=Position(0, 0),
		Position const & act_vec=Position(0, 0)
	) : 
		moveable(mov), move(disp), active(true), 
		action(act), actionPos(act_at), actionVec(act_vec)
	{}

	/*! Create a stroke from a JSON Dict. 
	    @param moveable the Moveable to assign
	 */
	Stroke(Moveable & mov, JSON::Dict const & json) : Stroke(mov) {
		if (ISLIST(json.get("move")))
			move = LIST(json.get("move"));
		if (ISINT(json.get("act"))){
			action = (ActionType) INT(json.get("act")).value();
			actionPos = LIST(json.get("actp"));
			actionVec = LIST(json.get("actv"));
		}
	}

	operator JSON::Dict() const {
		JSON::Dict res = {
			{"mid", JSON::Integer(moveable.getID())}, 
			{"move", JSON::List(move)}
		};
		if (action != ACT_NONE){
			res.set("act", (int) action);
			res.set("actp", JSON::List(actionPos));
			res.set("actv", JSON::List(actionVec));
		}
		return res;
	}
};

#endif