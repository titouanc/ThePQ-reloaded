#include "Player.hpp"

Player::Player(JSON::Dict const & json) : Member(json), Moveable(json){
        if (ISINT(json.get("maxLife"))) _maxLife = INT(json.get("maxLife")).value();
        if (ISINT(json.get("maxMana"))) _maxMana = INT(json.get("maxMana")).value();
        if (ISINT(json.get("lifeBar"))) _lifeBar = INT(json.get("lifeBar")).value();
        if (ISINT(json.get("manaBar"))) _manaBar = INT(json.get("manaBar")).value();
        if (ISDICT(json.get("broomstick"))) *_broomstick = DICT(json.get("broomstick"));
        if (ISDICT(json.get("jersey"))) *_jersey = DICT(json.get("jersey"));
        if (ISINT(json.get("strength"))) _strength = INT(json.get("strength")).value();
        if (ISINT(json.get("constitution"))) _constitution = INT(json.get("constitution")).value();
        if (ISINT(json.get("magic"))) _magic = INT(json.get("magic")).value();
        if (ISINT(json.get("spirit"))) _spirit = INT(json.get("spirit")).value();
        if (ISINT(json.get("velocity"))) _velocity = INT(json.get("velocity")).value();
        if (ISINT(json.get("precision"))) _precision = INT(json.get("precision")).value();
        if (ISINT(json.get("chance"))) _chance = INT(json.get("chance")).value();
}

Player::operator JSON::Dict(){
    JSON::Dict res = (Member)*this; // TODO add keys from Moveable
    res.set("maxLife", _maxLife);
    res.set("maxMana", _maxMana);
    res.set("lifeBar", _lifeBar);
    res.set("manaBar", _manaBar);
    res.set("broomstick", JSON::Dict(*_broomstick));
    res.set("jersey", JSON::Dict(*_jersey));
    res.set("strength", _strength);
    res.set("constitution", _constitution);
    res.set("magic", _magic);
    res.set("spirit", _spirit);
    res.set("velocity", _velocity);
    res.set("precision", _precision);
    res.set("chance", _precision);
}



void Player::equipBroomstick (Broomstick aBroom){
    improveVelocity(-1 * _broomstick->getVelocityBonus());
    improveVelocity(aBroom.getVelocityBonus());
    *_broomstick = aBroom;
}

void Player::equipJersey (Jersey aJersey){
    improveStrength(-1 * _jersey->getStrengthBonus());
    improveConstitution(-1 * _jersey->getConstitutionBonus());
    improveMagic(-1 * _jersey->getMagicBonus());
    improveSpirit(-1 * _jersey->getSpiritBonus());
    improveStrength(aJersey.getStrengthBonus());
    improveConstitution(aJersey.getConstitutionBonus());
    improveMagic(aJersey.getMagicBonus());
    improveSpirit(aJersey.getSpiritBonus());
    *_jersey = aJersey;
}

void Player::recoverLife (){
    _lifeBar = _maxLife;
}

void Player::loseLife (int damage){
    _lifeBar -= damage;
    if (_lifeBar < 0)
        _lifeBar = 0 ;
}

void Player::recoverMana (){
    _manaBar = _maxMana;
}

void Player::loseMana (int spelled){
    _manaBar -= spelled;
    if (_manaBar < 0)
        _manaBar = 0;
}

// BEATER ----------------------------------------------------------------------
void Beater::equipBat(Bat aBat){
    _strength += (aBat.getStrengthBonus());
    _precision += (aBat.getPrecisionBonus());
    *_bat = aBat;
}

void Beater::unequipBat(){
    _strength -= _bat->getStrengthBonus();
    _precision -= _bat->getPrecisionBonus();
}

int Beater::collision (){
    return static_cast<int>( (float) getStrength() * .85 + (float) getMagic() * .1 + (float) getChance() * .05) * 100; 
}
int Beater::anticollision (){
    return static_cast<int>( (float) getConstitution() * .85 + (float) getMagic() * .1 + (float) getChance() * .05) * 100;
}

int Beater::shootBludger (){ 
    return static_cast<int>( (float) getStrength() * .5 + (float) getPrecision() * .35 + (float) getMagic() * .1 + (float) getChance() * .05 ) * 100;
}

// CHASER ----------------------------------------------------------------------
int Chaser::speed (){
    return static_cast<int>( (float) getVelocity() * .85 + (float) getStrength() * .1 + (float) getChance() * .05 ) * 100;
}

int Chaser::collisionner (){
    return static_cast<int>( (float) getStrength() * .65 + (float) getMagic() * .3 + (float) getChance() * .05 ) * 100;
}

int Chaser::anticollision (){
    return static_cast<int>( (float) getConstitution() * .65 + (float) getSpirit() * .3 + (float) getChance() * .05 ) * 100 ;
}

int Chaser::pass (){
    return static_cast<int>( (float) getStrength() * .4 + (float) getPrecision() * .5 + (float) getChance() * 0.1 ) * 100 ;
}

int Chaser::shoot (){
    return static_cast<int>( (float) getStrength() * .6 + (float) getPrecision() * .35 + (float) getChance() * .05 ) *100 ;
}

// KEEPER ----------------------------------------------------------------------
int Keeper::catchBall (){
    return static_cast<int>( (float) getVelocity() * .65 + (float) getPrecision() * .3 + (float) getChance() * .05 ) * 100;
}

int Keeper::pass (){
    return static_cast<int>( (float) getStrength() * .65 + (float) getPrecision() * .25 + (float) getChance() * .1) * 100 ;
}

// KEEPER ----------------------------------------------------------------------
int Seeker::catchGS (){
    return static_cast<int>( (float) getPrecision() * .50 + (float) getVelocity() * .20 + (float) getStrength() * .10 + (float) getMagic() * .1 + (float) getChance() * .1 ) * 100 ;
}




