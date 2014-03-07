#ifndef DEFINE_PITCH_HEADER
#define DEFINE_PITCH_HEADER 

#include "Moveable.hpp"
#include "PosMatrix.hpp"

class Pitch : public PosMatrix<Moveable> {
	private:
		/* Pitch dimensions */
		size_t _width, _height;
	public:
		typedef typename PosMatrix<Moveable>::iterator iterator;
		typedef typename PosMatrix<Moveable>::const_iterator const_iterator;
		static const Position West;
		static const Position East;
		static const Position NorthWest;
		static const Position SouthWest;
		static const Position NorthEast;
		static const Position SouthEast;
		static const Position directions[6];
		static const std::string directions_names[6];

		Pitch(size_t width = 100, size_t height = 36);
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
		using PosMatrix<Moveable>::getAt;
		Moveable * getAt(int x, int y) const;

		/*!
		 * @meth void Pitch::setAt(int x, int y, Moveable *moveable)
		 * @brief Set Moveable at position x,y
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 * @param moveable The moveable to place on the pitch
		 */
		using PosMatrix<Moveable>::setAt;
		void setAt(int x, int y, Moveable *moveable);

		/*!
		 * @meth bool Pitch::inEllipsis(int x, int y) const
		 * @brief Return true if pos x,y is in the ellipsoidal pitch
		 * @param x The column (the middle column is 0)
		 * @param y The row (the middle row is 0)
		 */
		bool inEllipsis(int x, int y) const;
		bool inEllipsis(Position position);
		bool inEllipsis(Moveable *moveable) const;

		/* Return true if (x,y) is in a keeper zone */
		bool isInWestKeeperZone(int x, int y) const;
		bool isInWestKeeperZone(Moveable *moveable) const;
		bool isInEastKeeperZone(int x, int y) const;
		bool isInEastKeeperZone(Moveable *moveable) const;
		bool isInKeeperZone(int x, int y) const {
			return isInEastKeeperZone(x, y) || isInWestKeeperZone(x, y);
		}
		bool isInKeeperZone(Position const & pos) const {
			return isInKeeperZone(pos.x(), pos.y());
		}

		/* Return true if (x,y) is an hexagon in the rectangular pitch area */
		bool isValid(int x, int y) const {return abs(x%2) == abs(y%2);}
		bool isValid(Position const & pos) const {
			return isValid(pos.x(), pos.y());
		}

		/* Return true if (x,y) is in a goal */
		bool isWestGoal(int x, int y) const;
		bool isEastGoal(int x, int y) const;
		bool isGoal(int x, int y) const {
			return isEastGoal(x, y) || isWestGoal(x, y);
		}
		bool isGoal(Position const & pos) const {
			return isGoal(pos.x(), pos.y());
		}

		// TODO DOC
		std::vector<Position> freePositionsAround(Position &position);

		/*!
		 * @meth void Pitch::insert(Moveable *moveable)
		 * @brief Set a moveable to its position on pitch
		 * @param moveable The moveable to place on the pitch
		 */
		bool insert(Moveable *moveable);
};

std::ostream & operator<<(std::ostream & out, Pitch const & pitch);

#endif
