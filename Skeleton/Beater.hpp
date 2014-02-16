#ifndef Beater_hpp
#define Beater_hpp

#include "Player.hpp"
#include "Bat.hpp"

class Beater : public Player 
{
public:
	bool isBeater () const;
	void equipBat (Bat aBat);
    void unequipBat ();
    int collision ();
    int anticollision ();
    int shootBludger ();
private:
	Bat * bat;
};

#endif
