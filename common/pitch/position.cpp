#include "position.hpp"
#include <cassert>

Position::Position(int x, int y) : _x(x), _y(y)
{}

Position::Position(Position const & other) : _x(other.x()), _y(other.y())
{}

Position::Position(Position const && other): _x(other.x()), _y(other.y())
{}

Position::Position(JSON::List const & list)
{
    assert(list.len() == 2 && ISINT(list[0]) && ISINT(list[1]));
    _x = INT(list[0]).value();
    _y = INT(list[1]).value();
}

int Position::x(void) const 
{
	return _x;
}

int Position::y(void) const 
{
	return _y;
}

unsigned int Position::length(void) const 
{
	return (abs(_x)+abs(_y))/2;
}

bool Position::isDirection(void) const
{
    return (abs(_x)==abs(_y) || abs(_y)==0) && (_x+_y)%2 == 0;
}

Position Position::operator+(Position const & other) const
{
    return Position(_x+other.x(), _y+other.y());
}

Position const & Position::operator+=(Position const & other)
{
    _x += other.x();
    _y += other.y();
    return *this;
}

bool Position::operator==(Position const & other)
{
    return _x == other.x() && _y == other.y();
}

Position & Position::operator=(Position const & other)
{
    _x = other.x();
    _y = other.y();
    return *this;
}

JSON::List Position::toJson(void) const 
{
    JSON::List res;
    res.append(JSON::Integer(_x));
    res.append(JSON::Integer(_y));
    return res;
}
