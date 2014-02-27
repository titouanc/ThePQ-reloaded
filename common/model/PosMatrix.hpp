#ifndef DEFINE_POSMATRIX_HEADER
#define DEFINE_POSMATRIX_HEADER

#include <cstdint>
#include <unordered_map>
#include "Position.hpp"


/* 
	PosMatrix efficiently manages sparse matrices of any size that could fit
	in a Position.
*/

template <typename T>
class PosMatrix {
	private:
		typedef typename std::unordered_map<uint64_t, T*> map;
		map _content;
		uint64_t key(Position const & pos) const {
			return ((uint64_t) pos.x()<<32) | ((uint32_t) pos.y());
		}
	public:
		typedef typename map::iterator iterator;
		typedef typename map::const_iterator const_iterator;

		PosMatrix() : _content(){}

		~PosMatrix(){}

		T *getAt(Position const & pos) const {
			const_iterator it = _content.find(key(pos));
			return (it != _content.end()) ? it->second : NULL;
		}

		void setAt(Position const & pos, T *value) {
			iterator it = _content.find(key(pos));
			if (it != _content.end()){
				if (value != NULL)
					it->second = value;
				else
					_content.erase(it);
			} else if (value != NULL) {
				_content.emplace(key(pos), value);
			}
		}

		iterator begin(void) {return _content.begin();}
		iterator   end(void) {return _content.end();}
		const_iterator begin(void) const {return _content.begin();}
		const_iterator   end(void) const {return _content.end();}
};

#endif