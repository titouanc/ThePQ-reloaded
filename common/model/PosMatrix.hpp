#ifndef DEFINE_POSMATRIX_HEADER
#define DEFINE_POSMATRIX_HEADER

#include <cstdint>
#include <unordered_map>
#include <functional>
#include "Position.hpp"

/** 
 *   Define hash method for Position (needed for O(1) acces in PosMatrix)
 */
namespace std {
    template <>
    struct hash<Position>{
        std::size_t operator()(Position const & pos) const {
            using std::size_t;
            using std::hash;

            /* x in 4 upper bytes, y in 4 lower */
            unsigned long long key = ((unsigned long long) pos.x())<<32;
            key |= (uint32_t) pos.y();

            return hash<unsigned long long>()(key);
        }
    };
}


/** 
 *   PosMatrix efficiently manages sparse matrices of any size that could fit
 *  in a Position.
 */
template <typename T>
class PosMatrix : public std::unordered_map<Position, T*> {
    public:
        typedef typename std::unordered_map<Position, T*>::iterator iterator;
        typedef typename std::unordered_map<Position, T*>::const_iterator const_iterator;

        using std::unordered_map<Position, T*>::unordered_map;

        T *getAt(Position const & pos) const {
            const_iterator it = this->find(pos);
            return (it != this->end()) ? it->second : NULL;
        }

        void setAt(Position const & pos, T *value) {
            iterator it = this->find(pos);
            if (it != this->end()){
                if (value != NULL)
                    it->second = value;
                else
                    this->erase(it);
            } else if (value != NULL) {
                this->emplace(pos, value);
            }
        }
};

#endif