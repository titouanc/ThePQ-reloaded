#include "Abilities.hpp"

using namespace std;

Abilities::Abilities (){
}

int Abilities::getStrength (){
    return strength;
}

int Abilities::getConstitution (){
    return constitution;
}

int Abilities::getMagic (){
    return magic;
}

int Abilities::getSpirit (){
    return spirit;
}

int Abilities::getVelocity (){
    return velocity;
}

int Abilities::getPrecision (){
    return precision;
}

int Abilities::getChance (){
    return chance;
}

void Abilities::improveStrength (int added){
    strength+=added;
}

void Abilities::improveConstitution (int added){
    constitution+=added;
}

void Abilities::improveMagic (int added){
    magic+=added;
}

void Abilities::improveSpirit (int added){
    spirit+=added;
}

void Abilities::improveVelocity (int added){
    velocity+=added;
}

void Abilities::improvePrecision (int added){
    chance+=added;
}
