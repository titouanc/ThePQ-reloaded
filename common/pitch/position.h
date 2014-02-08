#ifndef DEFINE_POSITION_HEADER
#define DEFINE_POSITION_HEADER

#include <vector>
#include <json.h>
#include <cassert>

class Position {
    private:
        int _x, _y;
    public:
        Position(int x=0, int y=0) : _x(x), _y(y) {}
        Position(Position const & other) : _x(other.x()), _y(other.y()){}
        Position(Position const && other): _x(other.x()), _y(other.y()){}
        Position(JSON::List const & list){
            assert(list.len() == 2 && ISINT(list[0]) && ISINT(list[1]));
            _x = INT(list[0]).value();
            _y = INT(list[1]).value();
        }
        int x(void) const {return _x;}
        int y(void) const {return _y;}
        /* Return the length from the origin to this position, in cells */
        unsigned int length(void) const {return (abs(_x)+abs(_y))/2;}
        Position operator+(Position const & other){
            return Position(_x+other.x(), _y+other.y());
        }
        Position const & operator+=(Position const & other){
            _x += other.x();
            _y += other.y();
            return *this;
        }
        Position operator*(int scalar){
            return Position(_x*scalar, _y*scalar);
        }
        friend Position operator*(int scalar, Position & pos){
            return pos*scalar;
        }
        bool operator==(Position const & other){
            return _x == other.x() && _y == other.y();
        }
        Position & operator=(Position const & other){
            _x = other.x();
            _y = other.y();
            return *this;
        }
        JSON::List toJson(void) const {
            JSON::List res;
            res.append(JSON::Integer(_x));
            res.append(JSON::Integer(_y));
            return res;
        }
};

class Displacement {
    private:
        vector<Position> _moves;
    public:
        Displacement() : _moves(){}
        Position finalPosition(Position const & initial=Position(0, 0)) const {
            Position res(initial);
            for (size_t i=0; i<count(); i++)
                res += _moves[i];
            return res;
        }
        void addMove(Position const & move) {
            _moves.push_back(move);
        };
        size_t count(void) const {return _moves.size();}
        JSON::List toJson(void) const {
            JSON::List res;
            for (size_t i=0; i<count(); i++)
                res.append(_moves[i].toJson());
            return res;
        }
        unsigned int length(void) const {
            unsigned int res = 0;
            for (size_t i=0; i<count(); i++)
                res += _moves[i].length();
            return res;
        }
};
#endif
