#ifndef Player_hpp
#define Player_hpp

#include "Member.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"
#include "Moveable.hpp"

class Player : public Member, public Moveable 
{
public:
	Player ();
	int getRemainingLife () const;
	int getRemainingMana ()const;
    void equipBroomstick (Broomstick aBroom);
	Jersey equipJersey (Jersey aJersey);
	void recoverLife ();
	void loseLife (int damage);
	void recoverMana ();
	void loseMana (int spelled);
	bool isBeater () const;
	bool isChaser () const;
	bool isKeeper () const;
	bool isSeeker () const;
	int getStrength () const;
	int getConstitution () const;
	int getMagic () const;
	int getSpirit () const;
	int getVelocity () const;
	int getPrecision () const;
	int getChance () const;
	void improveStrength (int added);
	void improveConstitution (int added);
	void improveMagic (int added);
	void improveSpirit (int added);
	void improveVelocity (int added);
	void improvePrecision (int added);
protected:
    int maxLife;
    int maxMana;
	int lifeBar;
	int manaBar;
	Broomstick * broomstick;
    Jersey * jersey;
	int strength;
	int constitution;
	int magic;
	int spirit;
	int velocity;
	int precision;
	int chance;
};

#endif
