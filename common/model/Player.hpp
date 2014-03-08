#ifndef __PLAYER_HPP
#define __PLAYER_HPP

#include <json/json.hpp>
#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Member.hpp"
#include "Gear.hpp"
#include "Moveable.hpp"


class Player : public Member, public Moveable
{
	friend std::ostream& operator<< (std::ostream&, const Player&);//modif
public:
	// Player ():  Member(), _maxLife(100), _maxMana(100), _lifeBar(100), 
 //                _manaBar(100), _broomstick(new Broomstick(5,50)), 
 //                _jersey(new Jersey()), _strength(5), _constitution(5), 
 //                _magic(5), _spirit(5), _velocity(5), _precision(5), _chance(5){}

    Player(int id, std::string username): Player() {
    	_memberID = id;
    	_owner = username;
    }
    Player(JSON::Dict const & json = JSON::Dict());
    Player(const Player&);//modif
    ~Player(){
    	if (_broomstick != NULL) delete _broomstick; 
    	if (_jersey != NULL) delete _jersey; 
    }
    operator JSON::Dict() const;
    //void save();
    //Player* load(int id, std::string username = "");

	int getRemainingLife () const	{ return _lifeBar; }
	int getRemainingMana ()const	{ return _manaBar; }
	string getName() 	const    	{ return Member::getName(); }
    void setName(string name) { _name = name; }
    void equipBroomstick (Broomstick aBroom);
	void equipJersey (Jersey aJersey);
	void recoverLife ();
	void loseLife (int damage);
	void recoverMana ();
	void loseMana (int spelled);
	virtual bool isBeater () const { return false; }
	virtual bool isChaser () const { return false; }
	virtual bool isKeeper () const { return false; }
	virtual bool isSeeker () const { return false; }
	int getStrength () const 		{ return _strength + _jersey->getStrengthBonus(); }
	int getVelocity () const 		{ return _velocity + _broomstick->getVelocityBonus() ;}
	int getPrecision () const 		{ return _precision; }
	int getChance () const 			{ return _chance; }
    Broomstick getBroomstick () const { return *(_broomstick); }
    Jersey getJersey () const         { return *(_jersey);     }
    void setStrength(int strength) { _strength = strength; }
    void setVelocity(int velocity) { _velocity = velocity; }
    void setPrecision (int prec)   { _precision = prec;    }
    void setChance (int chance)    { _chance = chance;     }
	void improveStrength (int added) 		{ _strength+=added; }
	void improveVelocity (int added) 		{ _velocity+=added; }
	void improvePrecision (int added) 		{ _precision+=added; }
	int estimatedValue(){ return (_strength+_velocity+_precision+_chance)*1000;} //TODO
	float collisionScore() const;
    int level () const;

	Player &operator=(Player const & player){
		Moveable::operator=(player);
		Member::operator=(player);
		_maxLife = player._maxLife;
		_maxMana = player._maxMana;
		_lifeBar = player._lifeBar;
		_manaBar = player._manaBar;
		_broomstick = new Broomstick(*(player._broomstick));
		_jersey = new Jersey(*(player._jersey));
		_strength = player._strength;
		_velocity = player._velocity;
		_precision = player._precision;
		_chance = player._chance;
		return *this;
	}

    float getSpeed() const {
        float res = 4*getVelocity();
        res = res * (100000000 +(rand()%10000000))/100000000;
        return res;
    }

protected:
    int _maxLife;
    int _maxMana;
	int _lifeBar;
	int _manaBar;
	Broomstick * _broomstick;
    Jersey * _jersey;
	int _strength;
	int _velocity;
	int _precision;
	int _chance;
};



/*================================BEATER==============================*/
class Beater : public Player 
{
public:
    Beater() : Player(), _bat(new Bat(5, 5)){}
    Beater(const Beater & beater) : Player(beater), _bat(new Bat(*(beater._bat))){}//modif
    Beater(JSON::Dict const & json) : Player(json){
    	if (ISDICT(json.get("bat")))
    		_bat = new Bat(DICT(json.get("bat")));
    	else
    		_bat = new Bat(5, 5); /* TODO: Too much magic here !!! */
    }
	bool isBeater () const { return true; }
	int getStrength() const { return Player::getStrength() + _bat->getStrengthBonus() ; }
    int getPrecision() const { return Player::getPrecision() + _bat->getPrecisionBonus(); }
    void equipBat (Bat aBat);
    float shootBludger () const;
private:
	Bat * _bat;
};

/*================================CHASER===============================*/
class Chaser : public Player 
{
public:
	using Player::Player;
    bool isChaser () const { return true; }
    float pass () const;
    float shoot () const;
};

/*================================CHASER================================*/
class Keeper : public Player 
{
public:
    using Player::Player;
	bool isKeeper () const { return true; }
    float catchBall () const;
    float pass () const;
};

// SEEKER ----------------------------------------------------------------------
class Seeker : public Player 
{
public:
    using Player::Player;
	bool isSeeker () const { return true; }
    float catchGS () const;

};


#endif
