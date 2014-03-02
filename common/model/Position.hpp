#ifndef DEFINE_POSITION_HEADER
#define DEFINE_POSITION_HEADER

#include <cmath>
#include <json/json.hpp>

class Position {
    private:
        int _x, _y;
    public:
        /* Constructors */
        Position(int x=0, int y=0);
        Position(Position const & other);
        Position(Position const && other);
        Position(JSON::List const & list);
        Position & operator=(Position const & other);

        /* Accessors */
        int x(void) const;
        int y(void) const;
        bool operator==(Position const & other) const;
        bool operator!=(Position const & other) const {return ! (*this == other);}
        unsigned int length(void) const; //Distance, in cells, from 0,0 to this pos
        bool isDirection(void) const;

        /* Numeric operations */
        Position operator+(Position const & other) const;
        Position operator-(Position const & other) const;
        Position const & operator+=(Position const & other);
        
        /* Transformations */
        operator JSON::List (void) const;
};

template <typename T>
Position operator*(T scalar, Position const & pos) {
    return Position(round(scalar*pos.x()), round(scalar*pos.y()));
}

template <typename T>
Position operator*(Position const & pos, T scalar) {
    return scalar*pos;
}

#endif
