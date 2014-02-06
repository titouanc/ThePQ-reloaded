#ifndef Position_hpp
#define Position_hpp

#include "Constants.hpp"

class Position {
public:
	Position();
	Position(int row, int col);
	~Position();
	bool operator==(Position other);
	Position operator+(Displacement displacement);
	bool isWithinLimits();
private:
	int _row;
	int _col;
};

#endif
