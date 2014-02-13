#ifndef DEFINE_PITCH_HEADER
#define DEFINE_PITCH_HEADER 

#include "Moveable.hpp"

class Pitch {
	private:
		/* Pitch dimensions */
		size_t _width, _height;
		/* Matrix representation for fast position->player lookup */
		Moveable **_matrix;
		/* Translate x,y position to index in inner representation */
		size_t _index(int x, int y) const;
	public:
		Pitch(size_t width, size_t height);
		~Pitch();

		/*!
		 * @meth Moveable * Pitch::getAt(int x, int y) const
		 * @brief Get Moveable at position x,y
		 * @return The moveable at the xth column and yth row (NULL if nobody there)
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 */
		Moveable * getAt(int x, int y) const;

		bool inEllipsis(int x, int y) const;
		bool inEllipsis(Moveable *moveable) const;

		/*!
		 * @meth void Pitch::setAt(int x, int y, Moveable *moveable)
		 * @brief Set Moveable at position x,y
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 * @param moveable The moveable to place on the pitch
		 */
		void setAt(int x, int y, Moveable *moveable);
		void setAt(Position const & pos, Moveable *moveable);

		/*!
		 * @meth void Pitch::insert(Moveable *moveable)
		 * @brief Set a moveable to its position on pitch
		 * @param moveable The moveable to place on the pitch
		 */
		void insert(Moveable *moveable);
};

#endif
