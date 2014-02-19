#include "Pitch.hpp"
#include <cassert>
#include <cstring>

const Position Pitch::West(-2, 0);
const Position Pitch::East(2, 0);
const Position Pitch::NorthWest(-1, 1);
const Position Pitch::SouthWest(-1, -1);
const Position Pitch::NorthEast(1, 1);
const Position Pitch::SouthEast(1, -1);

size_t Pitch::_index(int x, int y) const 
{
	x += _width/2;
	y += _height/2;
	assert(0<=x && x<(int)_width);
	assert(0<=y && y<(int)_height);
	return y*_width + x;
}

Pitch::Pitch(size_t width, size_t height) : _width(width), _height(height)
{
	_matrix = new Moveable*[width*height];
	memset(_matrix, 0, width*height*sizeof(Moveable*));
}

Pitch::~Pitch()
{
	/*Destructor*/
	delete[] _matrix;
}
/*================Getters=====================*/
size_t Pitch::width(void) const {return _width;}
size_t Pitch::height(void) const {return _height;}

int Pitch::xmin(void) const {return -xmax();}
int Pitch::xmax(void) const {return _width/2;}

int Pitch::ymin(void) const {return -ymax();}
int Pitch::ymax(void) const {return _height/2;}

Position Pitch::center(void) const {return Position(0, 0);}

Moveable * Pitch::getAt(int x, int y) const 
{
	return _matrix[_index(x, y)];
}

Moveable * Pitch::getAt(Position const & pos) const
{
	return getAt(pos.x(), pos.y());
}

bool Pitch::inEllipsis(int x, int y) const
{
	double k = ((double) _width)/((double)_height);
	double L = xmax(); 
	return (x*x + k*k*y*y) < L*L;
}

bool Pitch::inEllipsis(Position position)
{
	return inEllipsis(position.x(), position.y());
}

bool Pitch::inEllipsis(Moveable *moveable) const
{
	/*Method checking position is inside the pitch*/
	return inEllipsis(moveable->getPosition().x(), moveable->getPosition().y());
}

bool Pitch::isInWestKeeperZone(int x, int y) const{
	bool res = false;
	if (inEllipsis(x, y) && (x < -int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInWestKeeperZone(Moveable *moveable) const{
	bool res = false;
	if (inEllipsis(moveable) && (moveable->getPosition().x() < -int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInEastKeeperZone(int x, int y) const{
	bool res = false;
	if (inEllipsis(x, y) && (x > int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInEastKeeperZone(Moveable *moveable) const{
	bool res = false;
	if (inEllipsis(moveable) && (moveable->getPosition().x() > int(width()/3.0)))
		res = true;
	return res;
}

bool Pitch::isWestGoal(int x, int y) const {
	bool res = false;
	if (inEllipsis(x, y) && x == -int(20.5*width()/48.0) && y < 3 && y > -3)
		res = true;
	return res;
}

bool Pitch::isEastGoal(int x, int y) const {
	bool res = false;
	if (inEllipsis(x, y) && x == int(20.5*width()/48.0) && y < 3 && y > -3)
		res = true;
	return res;
}

vector<Position> Pitch::freePositionsAround(Position &position){
	vector<Position> res;
	if (inEllipsis(position+West) && getAt(position+West) == NULL) res.push_back(position+West);
	if (inEllipsis(position+NorthWest) && getAt(position+NorthWest) == NULL) res.push_back(position+NorthWest);
	if (inEllipsis(position+NorthEast) && getAt(position+NorthEast) == NULL) res.push_back(position+NorthEast);
	if (inEllipsis(position+East) && getAt(position+East) == NULL) res.push_back(position+East);
	if (inEllipsis(position+SouthEast) && getAt(position+SouthEast) == NULL) res.push_back(position+SouthEast);
	if (inEllipsis(position+SouthWest) && getAt(position+SouthWest) == NULL) res.push_back(position+SouthWest);
	return res;
}

bool Pitch::setAt(int x, int y, Moveable *moveable)
{
	/*Method checking position is correct insisde the pitch
	 * and setting at position <<<x,y>>> if position is valid
	 */
	if (inEllipsis(x, y)){
		_matrix[_index(x, y)] = moveable;
		return true;
	}
	return false;
}

bool Pitch::setAt(Position const & pos, Moveable *moveable)
{
	/*Method checking position is correct insisde the pitch
	 * and setting at position <<<x,y>>> if position is valid*/
	return setAt(pos.x(), pos.y(), moveable);
}

bool Pitch::insert(Moveable *moveable)
{
	/*Method checking position is correct insisde the pitch
	 * and setting at position <<<x,y>>> if position is valid
	 */
	return setAt(moveable->getPosition(), moveable);
}

std::ostream & operator<<(std::ostream & out, Pitch const & pitch)
{
	/*Method for writing to the outputstream*/
	for (int y=pitch.ymax()-1; y>=pitch.ymin(); y--){
		for (int x=pitch.xmin(); x<pitch.xmax(); x++){
			if (abs(x%2) != abs(y%2 )|| ! pitch.inEllipsis(x, y)){
				out << " ";
			} else {
				if (pitch.getAt(x, y) == NULL){
					if (pitch.isInEastKeeperZone(x, y) || pitch.isInWestKeeperZone(x, y))
						if (pitch.isWestGoal(x, y) || pitch.isEastGoal(x, y))
							out << "O";
						else
							out << ".";
					else
						out << "\033[32m.\033[0m";
				} else {
					out << "\033[31mo\033[0m";
				}
			}
		}
		out << endl;
	}
	return out;
}
