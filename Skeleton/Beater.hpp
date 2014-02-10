#ifndef Beater_hpp
#define Beater_hpp

#include "Player.hpp"
#include "Bat.hpp"

class Beater : Player 
{
public:
	bool isBeater () const;
	void equipBat (Bat aBat);
    void unequipBat ();
private:
	Bat bat;
};

#endif
