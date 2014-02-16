#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include "Member.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"
#include "Moveable.hpp"

class Player : public Member, public Moveable 
{
public:
	Player ():  Member(), _maxLife(100), _maxMana(100), _lifeBar(100), 
                _manaBar(100), _broomstick(new Broomstick(5,50)), 
                _jersey(new Jersey()), _strength(5), _constitution(5), 
                _magic(5), _spirit(5), _velocity(5), _precision(5), _chance(5){}

	int getRemainingLife () const;	{ return _lifeBar; }
	int getRemainingMana ()const;	{ return _manaBar; }
	string getName() const 			{ return Member::getName(); }
    void equipBroomstick (Broomstick aBroom);
	Jersey equipJersey (Jersey aJersey);
	void recoverLife ();
	void loseLife (int damage);
	void recoverMana ();
	void loseMana (int spelled);
	virtual bool isBeater () const { return false; }
	virtual bool isChaser () const { return false; }
	virtual bool isKeeper () const { return false; }
	virtual bool isSeeker () const { return false; }
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
