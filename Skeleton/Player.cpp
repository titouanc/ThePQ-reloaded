#include "Player.hpp"
#include "Member.hpp"
#include "Player.hpp"
#include "Broomstick.hpp"
#include "Jersey.hpp"

Player::Player ():name ("Lasty Cotter"), maxLife(100), maxMana(100), lifeBar(100), manaBar(100), broomstick(new Broomstick(5,50)), jersey(new Jersey()), strength(5), constitution(5), magic(5), spirit(5), velocity(5), precision(5), chance(5){
    
}

int Player::getRemainingLife () const{
    return lifeBar;
}

int Player::getRemainingMana () const{
    return manaBar;
}

std::string Player::getName() const{
    return Member::getName();
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

int Player::getStrength () const{
    return strength;
}

int Player::getConstitution () const{
    return constitution;
}

int Player::getMagic () const{
    return magic;
}

int Player::getSpirit () const{
    return spirit;
}

int Player::getVelocity () const{
    return velocity;
}

int Player::getPrecision () const{
    return precision;
}

int Player::getChance () const{
    return chance;
}

void Player::improveStrength (int added){
    strength+=added;
}

void Player::improveConstitution (int added){
    constitution+=added;
}

void Player::improveMagic (int added){
    magic+=added;
}

void Player::improveSpirit (int added){
    spirit+=added;
}

void Player::improveVelocity (int added){
    velocity+=added;
}

void Player::improvePrecision (int added){
    chance+=added;
}
