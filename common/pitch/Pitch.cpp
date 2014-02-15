#include "Pitch.hpp"
#include <cassert>
#include <cstring>

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
	delete[] _matrix;
}

size_t Pitch::width(void) const {return _width;}
size_t Pitch::height(void) const {return _height;}

int Pitch::xmin(void) const {return -xmax();}
int Pitch::xmax(void) const {return _width/2;}

int Pitch::ymin(void) const {return -ymax();}
int Pitch::ymax(void) const {return _height/2;}

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
	double k = ((double) _width)/_height;
	double L = xmax();
	return (x*x + k*k*y*y) <= L*L;
}

bool Pitch::inEllipsis(Moveable *moveable) const
{
	return inEllipsis(moveable->getPosition().x(), moveable->getPosition().y());
}

bool Pitch::setAt(int x, int y, Moveable *moveable)
{
	if (inEllipsis(x, y)){
		_matrix[_index(x, y)] = moveable;
		return true;
	}
	return false;
}

bool Pitch::setAt(Position const & pos, Moveable *moveable)
{
	return setAt(pos.x(), pos.y(), moveable);
}

bool Pitch::insert(Moveable *moveable)
{
	return setAt(moveable->getPosition(), moveable);
}

std::ostream & operator<<(std::ostream & out, Pitch const & pitch)
{
	for (int y=pitch.ymax()-1; y>=pitch.ymin(); y--){
		for (int x=pitch.xmin(); x<pitch.xmax(); x++){
			if (abs(x%2) != abs(y%2 )|| ! pitch.inEllipsis(x, y)){
				out << " ";
			} else {
				if (pitch.getAt(x, y) == NULL){
					out << ".";
				} else {
					out << "\033[31mo\033[0m";
				}
			}
		}
		out << endl;
	}
	return out;
}
