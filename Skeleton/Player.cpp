#include "Player.hpp"
#include "Member.hpp"
#include "Abilities.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"

Player::Player (){

}

int Player::getRemainingLife () const{
    return lifeBar;
}

int Player::getRemainingMana () const{
    return manaBar;
}

Abilities Player::getAbilities () const{
    return abilities;
}

Broomstick Player::getBroomstick (){
    return broomstick;
}

void Player::equipBroomstick (Broomstick aBroom){
    Player::unequipBroomstick();
    abilities.improveVelocity(aBroom.getVelocity);
}

void unequipBroomstick (){
    if (broomstick != NULL) //TOFIX
        abilities.improveVelocity(-1 * broomstick.getVelocity());
}

Jersey Player::equipJersey (Jersey aJersey){
    abilities.improveStrength(-1 * jersey.getStrengthBonus());
    abilities.improveConstitution(-1 * jersey.getConstitutionBonus());
    abilities.improveMagic(-1 * jersey.getMagicBonus());
    abilities.improveSpirit(-1 * jersey.getSpiritBonus());
    abilities.improveStrength(aJersey.getStrengthBonus());
    abilities.improveConstitution(aJersey.getConstitutionBonus());
    abilities.improveMagic(aJersey.getMagicBonus());
    abilities.improveSpirit(aJersey.getSpiritBonus());
}

void Player::recoverLife (){
    lifeBar = manaBar;
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

bool Player::isBeater () const{
    return false;
}

bool Player::isChaser () const{
    return false;
}

bool Player::isKeeper () const{
    return false;
}

bool Player::isSeeker () const{
    return false;
}
