#ifndef Player_hpp
#define Player_hpp

#include "Member.hpp"
#include "Abilities.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"

class Player : Member 
{
public:
	Player ();
	int getRemainingLife ();
	int getRemainingMana ();
	Abilities getAbilities ();
	Broomstick getBroomstick ();
	Jersey equipJersey ();
	void recoverLife ();
	void loseLife (int damage);
	void recoverMana ();
	void loseMana (int spelled);
	void setBroomstick (Broomstick anotherBs);
	bool isBeater ();
	bool isChaser ();
	bool isKeeper ();
	bool isSeeker ();
private:
	int lifeBar;
	int manaBar;
	Abilities abilities;
	Broomstick broomstick;
    Jersey jersey;
};

#endif
