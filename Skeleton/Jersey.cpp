#include "Jersey.hpp"
#include "Gear.hpp"
#include "Player.hpp"

using namespace std;

int Jersey::getStrengthBonus () const{
    return strengthBonus;
}

int Jersey::getConstitutionBonus (){
    return constitutionBonus;
}

int Jersey::getMagicBonus (){
    return magicBonus;
}

int Jersey::getSpiritBonus (){
    return spiritBonus;
}

