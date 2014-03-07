#include "Pitch.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include <cassert>
#include <cstring>

const Position Pitch::West(-2, 0);
const Position Pitch::East(2, 0);
const Position Pitch::NorthWest(-1, 1);
const Position Pitch::SouthWest(-1, -1);
const Position Pitch::NorthEast(1, 1);
const Position Pitch::SouthEast(1, -1);
const Position Pitch::directions[6] = {
	Pitch::West, 
	Pitch::NorthWest, 
	Pitch::NorthEast, 
	Pitch::East, 
	Pitch::SouthEast, 
	Pitch::SouthWest
};

const std::string Pitch::directions_names[6] = {
	"West", "NorthWest", "NorthEast", "East", "SouthEast", "SouthWest"
};

Pitch::Pitch(size_t width, size_t height) : 
PosMatrix<Moveable>(), _width(width), _height(height)
{}

Pitch::~Pitch()
{}

/*================Getters=====================*/
size_t Pitch::width(void) const {return _width;}
size_t Pitch::height(void) const {return _height;}

int Pitch::xmin(void) const {return -xmax();}
int Pitch::xmax(void) const {return _width/2;}

int Pitch::ymin(void) const {return -ymax();}
int Pitch::ymax(void) const {return _height/2;}

Position Pitch::center(void) const {return Position(0, 0);}

Moveable * Pitch::getAt(int x, int y) const 
{
	return getAt(Position(x, y));
}

bool Pitch::inEllipsis(int x, int y) const
{
	double k = ((double) _width)/((double)_height);
	double L = xmax(); 
	return (x*x + k*k*y*y) < L*L;
}

bool Pitch::inEllipsis(Position position)
{
	return inEllipsis(position.x(), position.y());
}

bool Pitch::inEllipsis(Moveable *moveable) const
{
	/*Method checking position is inside the pitch*/
	return inEllipsis(moveable->getPosition().x(), moveable->getPosition().y());
}

bool Pitch::isInWestKeeperZone(int x, int y) const{
	bool res = false;
	if (inEllipsis(x, y) && (x < -int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInWestKeeperZone(Moveable *moveable) const{
	bool res = false;
	if (inEllipsis(moveable) && (moveable->getPosition().x() < -int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInEastKeeperZone(int x, int y) const{
	bool res = false;
	if (inEllipsis(x, y) && (x > int(width()/3.0)))
		res = true;
	return res;
}
bool Pitch::isInEastKeeperZone(Moveable *moveable) const{
	bool res = false;
	if (inEllipsis(moveable) && (moveable->getPosition().x() > int(width()/3.0)))
		res = true;
	return res;
}

bool Pitch::isWestGoal(int x, int y) const {
	bool res = false;
	if (inEllipsis(x, y) && x == -int(20.5*width()/48.0) && y < 3 && y > -3)
		res = true;
	return res;
}

bool Pitch::isEastGoal(int x, int y) const {
	bool res = false;
	if (inEllipsis(x, y) && x == int(20.5*width()/48.0) && y < 3 && y > -3)
		res = true;
	return res;
}

vector<Position> Pitch::freePositionsAround(Position &position){
	vector<Position> res;
	if (inEllipsis(position+West) && getAt(position+West) == NULL) 
		res.push_back(position+West);
	if (inEllipsis(position+NorthWest) && getAt(position+NorthWest) == NULL) 
		res.push_back(position+NorthWest);
	if (inEllipsis(position+NorthEast) && getAt(position+NorthEast) == NULL) 
		res.push_back(position+NorthEast);
	if (inEllipsis(position+East) && getAt(position+East) == NULL) 
		res.push_back(position+East);
	if (inEllipsis(position+SouthEast) && getAt(position+SouthEast) == NULL) 
		res.push_back(position+SouthEast);
	if (inEllipsis(position+SouthWest) && getAt(position+SouthWest) == NULL) 
		res.push_back(position+SouthWest);
	return res;
}

void Pitch::setAt(int x, int y, Moveable *moveable)
{
	setAt(Position(x, y), moveable);
}

bool Pitch::insert(Moveable *moveable)
{
	/*Method checking position is correct insisde the pitch
	 * and setting at position <<<x,y>>> if position is valid
	 */
	Position const & pos = moveable->getPosition();
	if (isValid(pos) && inEllipsis(pos)){
		setAt(pos, moveable);
		return true;
	}
	return false;
}

std::ostream & operator<<(std::ostream & out, Pitch const & pitch)
{
	/*Method for writing to the outputstream*/
	for (int y=pitch.ymax()-1; y>=pitch.ymin(); y--){
		for (int x=pitch.xmin(); x<pitch.xmax(); x++){
			if (abs(x%2) != abs(y%2 )|| ! pitch.inEllipsis(x, y)){
				out << " ";
			} else {
				Moveable *atPos = pitch.getAt(x, y);
				if (atPos == NULL){
					if (pitch.isInEastKeeperZone(x, y) || pitch.isInWestKeeperZone(x, y))
						if (pitch.isGoal(x, y))
							out << "O";
						else
							out << ".";
					else
						out << "\033[32m.\033[0m";
				} else if (atPos->isBall()) {
					Ball const & ball = (Ball const &) *atPos;
					if (ball.isGoldenSnitch())
						out << "\033[33m*\033[0m";
					else if (ball.isQuaffle())
						out << "\033[34m*\033[0m";
					else if (ball.isBludger())
						out << "\033[35m*\033[0m";
				} else if (atPos->isPlayer()) {
					out << "\033[31mo\033[0m";
				}
			}
		}
		out << endl;
	}
	return out;
}
