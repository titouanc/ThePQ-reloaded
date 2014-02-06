#ifndef Action_hpp
#define Action_hpp

class Action {
public:
	Action();
	~Action();
	bool isLegal();

private:
	Direction _direction;
};

#endif
