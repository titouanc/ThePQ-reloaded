#include "displacement.h"
#include "Moveable.hpp"
#include <vector>

using namespace std;

Moveable default_moveable;
Displacement default_displacement;

struct Stroke {
	Moveable moveable;
	Displacement move;
	Stroke(Moveable & m, Displacement & d): moveable(m), move(d){}
};

void move(std::vector<Stroke> strokes)
{
	size_t n_strokes = strokes.size();
	Position *pos = new Position[n_strokes];
	size_t i, len, maxlen = 0;
	for (i=0; i<n_strokes; i++){
		pos[i] = strokes[i].moveable.getPosition();
		len = strokes[i].move.length();
		if (len > maxlen)
			maxlen = len;
	}

	cout << " === DEBUT DU TOUR ===" << endl;

	double t;
	for (len=1; len<=maxlen; len++){
		t = ((double) len)/maxlen;
		for (i=0; i<n_strokes; i++){
			Stroke & s = strokes[i];
			Position newpos = s.move.position(t, s.moveable.getPosition());
			cout << s.moveable.getName() << " " << pos[i].toJson() << " -> " << newpos.toJson() << endl;
			for (size_t j=0; j<i; j++)
				if (newpos == pos[j]){
					cout << "Collision: " << s.moveable.getName()
					     << " arrive en " << newpos.toJson() 
					     << " mais " << strokes[j].moveable.getName()
					     << " y est deja." << endl;
				} else {
					pos[i] = newpos;
				}
		}
	}

	cout << " === FIN DU TOUR ===" << endl;
}

std::vector<Stroke> loadFixture(const char *filename)
{
	std::vector<Stroke> res;
	JSON::Dict *dict = (JSON::Dict*) JSON::load(filename);
	if (! dict)
		return res;

	JSON::Dict::iterator it;
	for (it=dict->begin(); it!=dict->end(); it++){
		std::string const & name = it->first;
		if (ISDICT(it->second)){
			Position pos(LIST(DICT(it->second).get("pos")));
			Displacement move(LIST(DICT(it->second).get("move")));
			Moveable player(name, 0, 0, pos);
			res.push_back(Stroke(player, move));
		}
	}

	delete dict;
	return res;
}

int main(int argc, const char **argv)
{
	if (argc < 2){
		cout << "USAGE: " << argv[0] << " fichiers deplacements" << endl;
	}
	for (int i=1; i<argc; i++){
		std::vector<Stroke> const & test = loadFixture(argv[i]);
		move(test);
	}
}
