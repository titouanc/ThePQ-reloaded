#ifndef DEFINE_DISPLACEMENT_HEADER
#define DEFINE_DISPLACEMENT_HEADER

#include <json.h>
#include <vector>
#include <stdexcept>
#include "position.h"

class NotADirection : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Displacement {
    private:
        std::vector<Position> _moves;
    public:
        Displacement();
        Displacement(JSON::List const & list);
        Position position(double t=1, Position const & initial=Position(0, 0)) const;
        void addMove(Position const & move);
        size_t count(void) const;
        JSON::List toJson(void) const;
        unsigned int length(void) const;
};

#endif
