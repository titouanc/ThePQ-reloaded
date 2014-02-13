#ifndef DEFINE_MATCHMANAGER_HEADER
#define DEFINE_MATCHMANAGER_HEADER 

#include <cassert>
#include <queue>
#include <pitch/Moveable.hpp>
#include <pitch/Displacement.hpp>
#include <cstring>

class Player : public Moveable {
	using Moveable::Moveable;
};

class Ball : public Moveable {
	using Moveable::Moveable;
};

struct Stroke {
	Moveable & moveable;
	Displacement move;
};

struct Squad {
	Player *players[7];
	std::string managerName;
};

class Pitch {
	private:
		size_t _width, _height;
		Moveable **_matrix;
		size_t _index(int x, int y) const {
			x += _width/2;
			y += _height/2;
			assert(0<=x && x<(int)_width);
			assert(0<=y && y<(int)_height);
			return x*_width + y;
		}
	public:
		Pitch(size_t width, size_t height) : _width(width), _height(height){
			_matrix = new Moveable*[width*height];
			memset(_matrix, 0, width*height*sizeof(Moveable*));
		}
		~Pitch(){delete[] _matrix;}

		Moveable * getAt(int x, int y) const {
			return _matrix[_index(x, y)];
		}
		void setAt(int x, int y, Moveable *moveable){
			Position initial = moveable->getPosition();
			Position dest(x, y);
			size_t fromPos = _index(initial.x(), initial.y());
			if (! (initial == dest) && _matrix[fromPos] == moveable){
				_matrix[fromPos] = NULL;
				moveable->setPosition(dest);
			}
			_matrix[_index(x, y)] = moveable;
		}
};

class MatchManager {
	private:
		std::queue<Stroke> _strokes;
		Squad _squads[2];
		Ball *_balls[4];
		Pitch _pitch;
	public:
		MatchManager() : _strokes(), _pitch(100, 36) {
			memset(_squads, 0, 2*sizeof(Squad));
			memset(_balls, 0, 4*sizeof(Ball*));
		}
		~MatchManager();
		void onCollision(Stroke & s1, Stroke & s2, Position &conflict){
			std::cout << "Collision " << s1.moveable.getName() << " & "
			          << s2.moveable.getName() << " => " << conflict.toJson();
		}
};

#endif
