#ifndef Player_hpp
#define Player_hpp

#include "Member.hpp"
#include "Abilities.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"
#include "Moveable.hpp"

class Player : public Member, public Moveable 
{
public:
	Player ();
	int getRemainingLife () const;
	int getRemainingMana ()const;
	Abilities getAbilities () const;
	Broomstick getBroomstick () const;
    void equipBroomstick (Broomstick aBroom);
    void unequipBroomstick ();
	Jersey equipJersey ();
	void recoverLife ();
	void loseLife (int damage);
	void recoverMana ();
	void loseMana (int spelled);
	bool isBeater () const;
	bool isChaser () const;
	bool isKeeper () const;
	bool isSeeker () const;
private:
    int maxLife;
    int maxMana;
	int lifeBar;
	int manaBar;
	Abilities abilities;
	Broomstick broomstick;
    Jersey jersey;
};

#endif
