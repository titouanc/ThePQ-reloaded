#include "Displacement.hpp"
#include <cassert>

Displacement::Displacement(double tbegin) :
 _tbeg(tbegin), _moves(), _totalLength(0)
{}

Displacement::Displacement(JSON::List const & list) : Displacement() 
{
    for (size_t i=0; i<list.len(); i++){
        assert(ISLIST(list[i]));
        addMove(Position(LIST(list[i])));
    }
}

Position Displacement::position(double t, size_t speed) const 
{
    /*Method returning the calculated position*/
    assert(0 <= t && t <= 1);

    Position res;
    double begin=0, end=0;
    if (t < _tbeg)
        return res;

    t -= _tbeg;

    if (speed==0)
        speed = length(); /* If no speed given, take start->t=0; end->t=1*/
    assert(speed >= length());

    t *= speed;

    for (size_t i=0; i<count(); i++){
        /* Time interval for this direction */
        end = begin + _moves[i].length();

        if (end <= t){
            begin = end;
            res += _moves[i];
        } else {
            double l = t-begin;
            double relative_pos = l/(end-begin);
            return res + _moves[i]*relative_pos;
        }
    }
    return res;
}

Position Displacement::position(double t, Position const & initial) const
{
    /*Method returning the updated position*/
    return initial+position(t, length());
}

Position Displacement::position(double t, Moveable const & moveable) const 
{
    /*Method returning the updated position*/
    return moveable.getPosition()+position(t, moveable.getSpeed());
}

void Displacement::addMove(Position const & move) 
{
    /*Method for adding a move to the move queue*/
    if (! move.isDirection())
        throw NotADirection(((JSON::List)move).dumps());
    _moves.push_back(move);
    _totalLength += move.length();
}

size_t Displacement::count(void) const 
{
    /*Method returning the number of queued moves*/
    return _moves.size();
}

Displacement::operator JSON::List(void) const 
{
    /*Method for serializing Displacement to a JSON list*/
    JSON::List res;
    for (size_t i=0; i<count(); i++)
        res.append((JSON::List)_moves[i]);
    return res;
}

unsigned int Displacement::length(void) const 
{
    /*Method for calculating the total distance of the moves*/
    return _totalLength;
}
