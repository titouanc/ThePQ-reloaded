#include "Bat.hpp"
#include "Player.hpp"

using namespace std;

Bat::Bat(const int const strength, const int const precision): strengthBonus(strength),precisionBonus(precision){

}

int Bat::getStrengthBonus () const{
    return strengthBonus;
}

int Bat::getPrecisionBonus () const{
    return precisionBonus;
}

