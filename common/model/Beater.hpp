#ifndef __BEATER_HPP
#define __BEATER_HPP

#include "Player.hpp"
#include "Bat.hpp"

class Beater : public Player 
{
public:
    Beater();
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
