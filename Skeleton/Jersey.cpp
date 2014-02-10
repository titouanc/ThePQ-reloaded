#include "Jersey.hpp"
#include "Gear.hpp"
#include "Player.hpp"

using namespace std;

Jersey::Jersey():strengthBonus(0), constitutionBonus(0), magicBonus(0), spiritBonus(0){

}

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

