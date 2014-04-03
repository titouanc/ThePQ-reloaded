#include "Displacement.hpp"
#include <cassert>

/// Constructor
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

/**
 * Method calculating the new position
 * @param double : 
 * @param size_t : speed of the player
 * @return Position : new position 
 */
Position Displacement::position(double t, size_t speed) const 
{
    assert(0 <= t && t <= 1);
    Position res;
    double begin=0, end=0;
    if (t < _tbeg)
        return res;
    t -= _tbeg;

    if (speed==0)
        speed = length(); /* If no speed given, take start->t=0; end->t=1*/
    assert(speed >= length());

    t *= speed/(1-_tbeg);

    for (size_t i=0; i<count(); i++){
        /* Time interval for this direction */
        end = begin + _moves[i].length();

        if (end <= t){
            begin = end;
            res += _moves[i];
        } else {
            double l = t-begin;
            double relative_pos = l/(end-begin);
            if (_moves[i].y() == 0){
                /* If we travel on a horizontal line; ensure we're not
                   between 2 hexagons (axiom: parity line == parity column) */
                double x = res.x() + 2*round(_moves[i].x()*(relative_pos/2));
                return Position(x, res.y());
            } else {
                return res + _moves[i]*relative_pos;
            }
        }
    }
    return res;
}

/**
 * Method returning the updated position based on the initial position
 * @param double :
 * @param Position : initial position
 * @return Position : new position
 */
Position Displacement::position(double t, Position const & initial) const
{
    
    return initial+position(t, length());
}

/**
 * Method returning the updated position
 */
Position Displacement::position(double t, Moveable const & moveable) const 
{    
    return moveable.getPosition()+position(t, moveable.getSpeed());
}

/**
 * Method for adding a move to the move queue
 * @param Position : new position to be added to the queue
 */
void Displacement::addMove(Position const & move) 
{
    if (! move.isDirection())
        throw NotADirection(((JSON::List)move).dumps());
    _moves.push_back(move);
    _totalLength += move.length();
}

/**
 * Method returning the number of queued moves
 * @return size_t : number of queued moves
 */
size_t Displacement::count(void) const 
{
    return _moves.size();
}

/**
 * Method for serializing Displacement to a JSON list
 */
Displacement::operator JSON::List(void) const 
{
    JSON::List res;
    for (size_t i=0; i<count(); i++)
        res.append((JSON::List)_moves[i]);
    return res;
}

/**
 * Method for calculating the total distance of the moves
 */
unsigned int Displacement::length(void) const 
{
  
    return _totalLength;
}
