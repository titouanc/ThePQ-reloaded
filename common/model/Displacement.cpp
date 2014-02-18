#include "Displacement.hpp"
#include <cassert>

Displacement::Displacement() : _moves()
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
    assert(speed <= length());
    Position res;
    double begin=0, end=0;

    if (speed==0)
        speed = length();
    t *= speed;
    for (size_t i=0; i<count(); i++){
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
        throw NotADirection(move.toJson().dumps());
    _moves.push_back(move);
}

size_t Displacement::count(void) const 
{
    /*Method returning the number of queued moves*/
    return _moves.size();
}

JSON::List Displacement::toJson(void) const 
{
    /*Method for serializing Displacement to a JSON list*/
    JSON::List res;
    for (size_t i=0; i<count(); i++)
        res.append(_moves[i].toJson());
    return res;
}

unsigned int Displacement::length(void) const 
{
    /*Method for calculating the total distance of the moves*/
    unsigned int res = 0;
    for (size_t i=0; i<count(); i++){
        res += _moves[i].length();
    }
    return res;
}
