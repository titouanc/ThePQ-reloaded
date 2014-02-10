#include "Beater.hpp"
#include "Player.hpp"
#include "Bat.hpp"

using namespace std;

bool Beater::isBeater() const{
    return True;
}

void Beater::equipBat(Bat aBat){
    abilities.improveStrength(aBat.getStrengthBonus);
    abilities.improvePrecision(aBat.getPrecisionBonus);
}

void Beater::unequipBat(){
    abilities.improveStrength(-1 * bat.getStrengthBonus);
    abilities.improvePrecision(-1 * bat.getPrecisionBonus);
}
