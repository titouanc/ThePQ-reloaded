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
	int getStrength () const 		{ return _strength; }
	int getConstitution () const 	{ return _constitution; }
	int getMagic () const 			{ return _magic; }
	int getSpirit () const 			{ return _spirit; }
	int getVelocity () const 		{ return _velocity; }
	int getPrecision () const 		{ return _precision; }
	int getChance () const 			{ return _chance; }
	void improveStrength (int added) 		{ _strength+=added; }
	void improveConstitution (int added) 	{ _constitution+=added; }
	void improveMagic (int added) 			{ _magic+=added; }
	void improveSpirit (int added) 			{ _spirit+=added; }
	void improveVelocity (int added) 		{ _velocity+=added; }
	void improvePrecision (int added) 		{ _precision+=added; }
protected:
    int _maxLife;
    int _maxMana;
	int _lifeBar;
	int _manaBar;
	Broomstick * _broomstick;
    Jersey * _jersey;
	int _strength;
	int _constitution;
	int _magic;
	int _spirit;
	int _velocity;
	int _precision;
	int _chance;
};

#endif
