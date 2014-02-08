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

Position Displacement::position(float t, Position const & initial) const 
{
    assert(0 <= t && t <= 1);
    Position res(initial);
    float total_length=length(), begin=0, end=0;
    for (size_t i=0; i<count(); i++){
        end += _moves[i].length()/total_length;
        if (end < t){
            begin = end;
            res += _moves[i];
        } else {
            float relative_pos = (t-begin)/(end-begin);
            return res + _moves[i]*relative_pos;
        }
    }
    return res;
}

void Displacement::addMove(Position const & move) 
{
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
    for (size_t i=0; i<count(); i++)
        res += _moves[i].length();
    return res;
}
