#include "Installation.hpp"

int Installation::getMaintenanceCost(){
}

int Installation::getUpgradeCost(){
	// TODO test this with getDowngradeRefunds!
	float multiplicator = 1 + (getLevel()*0.1);
	return getCurrentValue() * multiplicator;
}

int Installation::getDowngradeRefunds(){
	// TODO test this with getUpgradeCost!
	float divider = 1 + ((getLevel()-1)*0.1);
	return getCurrentValue() / divider;
}

void Installation::upgrade(){
	if (getLevel() < getMaxLevel()){
		setCurrentValue(getUpgradeCost());
		setLevel( getLevel() + 1 );
	}
}

void Installation::downgrade(){
	if (getLevel() > 0){
		setCurrentValue(getDowngradeRefunds());
		setLevel( getLevel() - 1 );
	}
}