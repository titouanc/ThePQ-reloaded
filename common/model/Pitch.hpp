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

		size_t width(void) const;
		size_t height(void) const;

		int xmin(void) const;
		int xmax(void) const;

		int ymin(void) const;
		int ymax(void) const;

		/*!
		 * @meth Moveable * Pitch::getAt(int x, int y) const
		 * @brief Get Moveable at position x,y
		 * @return The moveable at the xth column and yth row (NULL if nobody there)
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 */
		Moveable * getAt(int x, int y) const;
		Moveable * getAt(Position const & pos) const;

		/*!
		 * @meth bool Pitch::inEllipsis(int x, int y) const
		 * @brief Return true if pos x,y is in the ellipsoidal pitch
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 */
		bool inEllipsis(int x, int y) const;
		bool inEllipsis(Moveable *moveable) const;

		/*!
		 * @meth void Pitch::setAt(int x, int y, Moveable *moveable)
		 * @brief Set Moveable at position x,y
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 * @param moveable The moveable to place on the pitch
		 */
		bool setAt(int x, int y, Moveable *moveable);
		bool setAt(Position const & pos, Moveable *moveable);

		/*!
		 * @meth void Pitch::insert(Moveable *moveable)
		 * @brief Set a moveable to its position on pitch
		 * @param moveable The moveable to place on the pitch
		 */
		bool insert(Moveable *moveable);
};

std::ostream & operator<<(std::ostream & out, Pitch const & pitch);

#endif