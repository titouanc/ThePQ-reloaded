#include "Beater.hpp"
#include "Player.hpp"
#include "Bat.hpp"

bool Beater::isBeater() const{
    return true;
}

void Beater::equipBat(Bat aBat){
    improveStrength(aBat.getStrengthBonus);
    improvePrecision(aBat.getPrecisionBonus);
}

void Beater::unequipBat(){
    improveStrength(-1 * bat->getStrengthBonus);
    improvePrecision(-1 * bat->getPrecisionBonus);
}
