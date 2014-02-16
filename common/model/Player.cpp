#include "Player.hpp"
#include "Member.hpp"
#include "Player.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"

Player::Player ():  Member(), maxLife(100), maxMana(100), lifeBar(100), 
                    manaBar(100), broomstick(new Broomstick(5,50)), 
                    jersey(new Jersey()), strength(5), constitution(5), 
                    magic(5), spirit(5), velocity(5), precision(5), chance(5){
    
}

void Player::equipBroomstick (Broomstick aBroom){
    improveVelocity(-1 * broomstick->getVelocityBonus());
    improveVelocity(aBroom.getVelocityBonus());
    *broomstick = aBroom;
}

Jersey Player::equipJersey (Jersey aJersey){
    improveStrength(-1 * jersey->getStrengthBonus());
    improveConstitution(-1 * jersey->getConstitutionBonus());
    improveMagic(-1 * jersey->getMagicBonus());
    improveSpirit(-1 * jersey->getSpiritBonus());
    improveStrength(aJersey.getStrengthBonus());
    improveConstitution(aJersey.getConstitutionBonus());
    improveMagic(aJersey.getMagicBonus());
    improveSpirit(aJersey.getSpiritBonus());
    *jersey = aJersey;
}

void Player::recoverLife (){
    lifeBar = maxLife;
}

void Player::loseLife (int damage){
    lifeBar -= damage;
    if (lifeBar < 0)
        lifeBar = 0 ;
}

void Player::recoverMana (){
    manaBar = maxMana;
}

void Player::loseMana (int spelled){
    manaBar -= spelled;
    if (manaBar < 0)
        manaBar = 0;
}
