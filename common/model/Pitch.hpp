#ifndef DEFINE_PITCH_HEADER
#define DEFINE_PITCH_HEADER 

#include "Moveable.hpp"
#include <vector>

class Pitch {
	private:
		/* Pitch dimensions */
		size_t _width, _height;
		/* Matrix representation for fast position->player lookup */
		Moveable **_matrix;
		/* Translate x,y position to index in inner representation */
		size_t _index(int x, int y) const;
	public:
		static const Position West;
		static const Position East;
		static const Position NorthWest;
		static const Position SouthWest;
		static const Position NorthEast;
		static const Position SouthEast;

		Pitch(size_t width, size_t height);
		~Pitch();

		size_t width(void) const;
		size_t height(void) const;

		int xmin(void) const;
		int xmax(void) const;

		int ymin(void) const;
		int ymax(void) const;

		Position center(void) const;

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
		bool inEllipsis(Position position);
		bool inEllipsis(Moveable *moveable) const;

		// TODO DOC
		bool isInWestKeeperZone(int x, int y) const;
		bool isInWestKeeperZone(Moveable *moveable) const;
		bool isInEastKeeperZone(int x, int y) const;
		bool isInEastKeeperZone(Moveable *moveable) const;

		// TODO DOC
		bool isWestGoal(int x, int y) const;
		bool isEastGoal(int x, int y) const;

		// TODO DOC
		std::vector<Position> freePositionsAround(Position &position);

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
