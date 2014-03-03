#ifndef DEFINE_DISPLACEMENT_HEADER
#define DEFINE_DISPLACEMENT_HEADER

#include <json/json.hpp>
#include <vector>
#include <stdexcept>
#include "Position.hpp"
#include "Moveable.hpp"

class NotADirection : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Displacement {
    private:
        /* Time of beginning */
        double _tbeg;
        /* Directions composing the displacement */
        std::vector<Position> _moves;
        unsigned int _totalLength;
    public:
        /* Create a new displacement starting at t=tbegin */
        Displacement(double tbegin=0);
        /* Load displacement from its JSON representation */
        Displacement(JSON::List const & list);

        /* Return the position  */
        Position position(double t=1, size_t speed=0) const;
        Position position(double t, Position const & initial) const;
        Position position(double t, Moveable const & moveable) const;

        /* Append a direction to the displacement */
        void addMove(Position const & move);

        /* Number of directions composing this displacement */
        size_t count(void) const;
        operator JSON::List(void) const;
        unsigned int length(void) const;
};

#endif
