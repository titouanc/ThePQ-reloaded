#ifndef DEFINE_POSMATRIX_HEADER
#define DEFINE_POSMATRIX_HEADER

#include <cstdint>
#include <unordered_map>
#include <functional>
#include "Position.hpp"

/* 
    Define hash method for Position (needed for O(1) acces in PosMatrix)
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


/* 
    PosMatrix efficiently manages sparse matrices of any size that could fit
    in a Position.
*/
template <typename T>
class PosMatrix {
    private:
        typedef typename std::unordered_map<Position, T*> map;
        map _content;
    public:
        typedef typename map::iterator iterator;
        typedef typename map::const_iterator const_iterator;

        PosMatrix() : _content(){}

        ~PosMatrix(){}

        T *getAt(Position const & pos) const {
            const_iterator it = _content.find(pos);
            return (it != _content.end()) ? it->second : NULL;
        }

        void setAt(Position const & pos, T *value) {
            iterator it = _content.find(pos);
            if (it != _content.end()){
                if (value != NULL)
                    it->second = value;
                else
                    _content.erase(it);
            } else if (value != NULL) {
                _content.emplace(pos, value);
            }
        }

        iterator begin(void) {return _content.begin();}
        iterator   end(void) {return _content.end();}
        const_iterator begin(void) const {return _content.begin();}
        const_iterator   end(void) const {return _content.end();}
};

#endif