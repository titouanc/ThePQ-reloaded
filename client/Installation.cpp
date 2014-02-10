#include "Installation.hpp"

int Installation::getMaintenanceCost(){
}

int Installation::getUpgradeCost(){
	float multiplicator = 1 + (getLevel()*0.1);
	return getCurrentValue() * multiplicator;
}

int Installation::getDowngradeRefunds(){
	float divider = 1 + ((getLevel()-1)*0.1);
	return getCurrentValue() / divider;
}

void Installation::upgrade(){
	if (getLevel() < getMaxLevel())
		setLevel( getLevel() + 1 );
}

void Installation::downgrade(){
	if (getLevel() > 0)
		setLevel( getLevel() - 1 );
}