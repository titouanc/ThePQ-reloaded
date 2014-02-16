#include "Player.hpp"

void Player::equipBroomstick (Broomstick aBroom){
    improveVelocity(-1 * _broomstick->getVelocityBonus());
    improveVelocity(aBroom.getVelocityBonus());
    *_broomstick = aBroom;
}

Jersey Player::equipJersey (Jersey aJersey){
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
    *bat = aBat;
}

void Beater::unequipBat(){
    _strength -= bat->getStrengthBonus();
    _precision -= bat->getPrecisionBonus();
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



