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
        std::vector<Position> _moves;
    public:
        Displacement();
        Displacement(JSON::List const & list);
        Position position(double t=1, size_t speed=0) const;
        Position position(double t, Position const & initial) const;
        Position position(double t, Moveable const & moveable) const;
        void addMove(Position const & move);
        size_t count(void) const;
        JSON::List toJson(void) const;
        unsigned int length(void) const;
};

#endif
