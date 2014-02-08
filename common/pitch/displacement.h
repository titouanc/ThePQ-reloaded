#ifndef DEFINE_DISPLACEMENT_HEADER
#define DEFINE_DISPLACEMENT_HEADER

#include <json.h>
#include <vector>
#include "position.h"

class Displacement {
    private:
        std::vector<Position> _moves;
    public:
        Displacement();
        Displacement(JSON::List const & list);
        Position position(float t=1, Position const & initial=Position(0, 0)) const;
        void addMove(Position const & move);
        size_t count(void) const;
        JSON::List toJson(void) const;
        unsigned int length(void) const;
};

#endif
