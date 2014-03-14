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
#include <Constants.hpp>

class Player : public Member, public Moveable
{
	friend std::ostream& operator<< (std::ostream&, const Player&);//modif
public:
	/* Default constructor + from JSON::Dict */
	Player(JSON::Dict const & json = JSON::Dict());
    
    /* Initializes a player with an id and owner */
    Player(int id, std::string username): Player() {
    	_memberID = id;
    	_owner = username;
    }

    Player(const Player&);
    operator JSON::Dict() const;
    Player& operator=(Player const & player);

    /* To delete; unused */
   	void recoverLife() 				{ _lifeBar = _maxLife;}
	void loseLife (int damage)		{ (damage > _lifeBar) ? _lifeBar = 0 : _lifeBar -= damage; }
	void recoverMana ()				{ _manaBar = _maxMana; }
	void loseMana (int mana)		{ (mana > _manaBar) ? _manaBar = 0 : _manaBar -= mana; }
	int getRemainingLife() const	{ return _lifeBar; }
	int getRemainingMana() const	{ return _manaBar; }

	/* Name handlers */
	std::string getName() const    	{ return Member::getName(); }
    void setName(string name) { _name = name; }
   
	/* Those methods are overridden in subclasses  */
	virtual bool isBeater () const { return false; }
	virtual bool isChaser () const { return false; }
	virtual bool isKeeper () const { return false; }
	virtual bool isSeeker () const { return false; }

	/* Aptitudes getters - per default, Player has no gear */
	int getStrength () const 		{ int ret; (_jersey == NULL) ? ret = _strength : ret = _strength + _jersey->getStrengthBonus(); return ret; }
	int getVelocity () const 		{ int ret; (_broomstick == NULL) ? ret = _velocity : ret = _velocity + _broomstick->getVelocityBonus(); return ret;}
	int getPrecision () const 		{ return _precision; }
	int getChance () const 			{ return _chance; }
	/* Effective aptitude(s) getter(s) used in a Match */
	float getSpeed() const 			{ return (getVelocity()+(gameconfig::MAXAPTVAL/10)-1)/(gameconfig::MAXAPTVAL/10); }
    
   	/* Aptitudes setters */
    void setStrength(int strength) { _strength = strength; }
    void setVelocity(int velocity) { _velocity = velocity; }
    void setPrecision (int prec)   { _precision = prec;    }
    void setChance (int chance)    { _chance = chance;     }
	void improveStrength (int added) 		{ _strength+=added; }
	void improveVelocity (int added) 		{ _velocity+=added; }
	void improvePrecision (int added) 		{ _precision+=added; }

	/* Utility */
	int estimatedValue(){ return (_strength+_velocity+_precision+_chance)*1000;}
	float collisionScore() const;
    int level () const;

    /* Gear setters - getters */
 	Broomstick* getBroomstick () 			{ return _broomstick; }
    Jersey* getJersey () 	     			{ return _jersey;     }
    void equipBroomstick (Broomstick* broom){ _broomstick = broom; }
	void equipJersey (Jersey* jersey) 		{ _jersey = jersey; }
	virtual void equipBat(Bat* bat) 		{}

    /*! return a string for this player role 
        ("beater", "chaser", "keeper" or "seeker") */
    virtual std::string getRole() const {return std::string("");}

    /*! return true if the player can catch/throw the quaffle */
    virtual bool canQuaffle() const { return false; }

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


/*! A base class for players that can catch and throw the quaffle */
class PlayerQuaffle : public Player
{
private:
    bool _hasQuaffle;
public:
    PlayerQuaffle & operator=(Player const & player);
    PlayerQuaffle(int id, std::string username) : Player(), _hasQuaffle(false) {}
    PlayerQuaffle(JSON::Dict const & json = JSON::Dict()) : Player(json) {
        _hasQuaffle = ISBOOL(json.get("Q?")) && BOOL(json.get("Q?"));
    }
    
    operator JSON::Dict() const {
        JSON::Dict res = Player::operator JSON::Dict();
        res.set("Q?", JSON::Bool(_hasQuaffle));
        return res;
    }

    bool canQuaffle() const { return true; }

    bool hasQuaffle() const {return _hasQuaffle;}
    void retainQuaffle(){_hasQuaffle = true;}
    void releaseQuaffle(){_hasQuaffle = false;}
};


/*================================BEATER==============================*/
class Beater : public Player 
{
public:
    Beater() : Player() {}
    
    Beater(Player& player) : Player(player) {}
    Beater(const Beater & beater) : Player(beater) {}
    Beater(JSON::Dict const & json) : Player(json){}
    Beater& operator=(Beater const & beater) { Player::operator=(beater); return *this; }

	bool isBeater () const 		{ return true; }
	int getStrength() const 	{ int ret; (_bat == NULL) ? ret = Player::getStrength() : ret = Player::getStrength() + _bat->getStrengthBonus(); return ret; }
    int getPrecision() const 	{ int ret; (_bat == NULL) ? ret = Player::getPrecision() : ret = Player::getPrecision() + _bat->getPrecisionBonus(); return ret; }
    void equipBat (Bat* bat)	{ _bat = bat; }
    Bat* getBat()				{ return _bat; }
    float shootBludger () const;
    std::string getRole() const {return std::string("Beater");}
private:
	Bat* _bat;
};

/*================================CHASER===============================*/
class Chaser : public PlayerQuaffle 
{
public:
	Chaser() : PlayerQuaffle() {}
	
	Chaser(Player& player) : PlayerQuaffle(player) {}
	Chaser(const Chaser & chaser) : PlayerQuaffle(chaser) {}
	Chaser(JSON::Dict const & json) : PlayerQuaffle(json){}
	Chaser& operator=(Chaser const & chaser) { PlayerQuaffle::operator=(chaser); return *this; }

    bool isChaser () const { return true; }
    /*! Return the pass score (speed of the quaffle when throwed) */
    float pass () const;
    float shoot() const;
    std::string getRole() const {return std::string("Chaser");}
};

/*================================KEEPER================================*/
class Keeper : public PlayerQuaffle 
{
public:
    using PlayerQuaffle::PlayerQuaffle;
    Keeper& operator=(Keeper const & keeper) { PlayerQuaffle::operator=(keeper); return *this; }
    Keeper& operator=(Player const & player) { PlayerQuaffle::operator=(player); return *this; }
	bool isKeeper () const { return true; }

    /*! Return the pass score (speed of the quaffle when throwed) */
    float pass () const;
    float catchBall() const;
    std::string getRole() const {return std::string("Keeper");}
};

/*================================SEEKER================================*/
class Seeker : public Player 
{
public:
    Seeker() : Player() {}
    
    Seeker(Player& player) : Player(player) {}
    Seeker(const Seeker & seeker) : Player(seeker) {}
    Seeker(JSON::Dict const & json) : Player(json){}
    Seeker& operator=(Seeker const & seeker) { Player::operator=(seeker); return *this; }

	bool isSeeker () const { return true; }

    /*! Return the catch golden snitch score 
        (probability to catch a Golden Snitch) */
    float catchGS () const;
    std::string getRole() const {return std::string("Seeker");}
};


#endif
