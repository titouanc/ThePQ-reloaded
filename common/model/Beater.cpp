#include "Beater.hpp"
#include "Player.hpp"
#include "Bat.hpp"

Beater::Beater() : Player(), bat(new Bat(5,5)){

}

bool Beater::isBeater() const{
    return true;
}

void Beater::equipBat(Bat aBat){
    strength += (aBat.getStrengthBonus());
    precision += (aBat.getPrecisionBonus());
    *bat = aBat;
}

void Beater::unequipBat(){
    strength -= bat->getStrengthBonus();
    precision -= bat->getPrecisionBonus();
}

int Beater::collision (){
    return static_cast<int>( (float) strength * .85 + (float) magic * .1 + (float) chance * .05) * 100; 
}
int Beater::anticollision (){
    return static_cast<int>( (float) constitution * .85 + (float) magic * .1 + (float) chance * .05) * 100;
}

int Beater::shootBludger (){ 
    return static_cast<int>( (float) strength * .5 + (float) precision * .35 + (float) magic * .1 + (float) chance * .05 ) * 100;
}

