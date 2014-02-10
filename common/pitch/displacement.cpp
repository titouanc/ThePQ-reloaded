#include "displacement.h"
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

Position Displacement::position(double t, Position const & initial) const 
{
    assert(0 <= t && t <= 1);
    Position res(initial);
    double begin=0, end=0;
    t *= length();
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

void Displacement::addMove(Position const & move) 
{
    if (! move.isDirection())
        throw NotADirection(move.toJson().dumps());
    _moves.push_back(move);
}

size_t Displacement::count(void) const 
{
    return _moves.size();
}

JSON::List Displacement::toJson(void) const 
{
    JSON::List res;
    for (size_t i=0; i<count(); i++)
        res.append(_moves[i].toJson());
    return res;
}

unsigned int Displacement::length(void) const 
{
    unsigned int res = 0;
    for (size_t i=0; i<count(); i++){
        res += _moves[i].length();
    }
    return res;
}
