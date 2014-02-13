#include "Pitch.hpp"
#include <cassert>
#include <cstring>

size_t Pitch::_index(int x, int y) const 
{
	x += _width/2;
	y += _height/2;
	assert(0<=x && x<(int)_width);
	assert(0<=y && y<(int)_height);
	return x*_width + y;
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

Moveable * Pitch::getAt(int x, int y) const 
{
	return _matrix[_index(x, y)];
}

void Pitch::setAt(int x, int y, Moveable *moveable)
{
	Position initial = moveable->getPosition();
	Position dest(x, y);
	size_t fromPos = _index(initial.x(), initial.y());
	if (initial != dest && _matrix[fromPos] == moveable){
		_matrix[fromPos] = NULL;
		moveable->setPosition(dest);
	}
	_matrix[_index(x, y)] = moveable;
}