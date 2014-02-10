#include "Installation.hpp"
#include <cmath>

int Installation::getMaintenanceCost(){
	// TODO decide how to compute maintenance costs.
	return 10; // TODO delete this magic number and implement a nice algorithm.
}

int Installation::getValueAtLevel(int level){
	return level ? (getBaseValue() * pow(2, level-1)) : 0;
}

int Installation::getUpgradeCost(){
	// returns a positive value that represents the cost of an upgrade to
	// the next level.
	int res = getBaseValue();	// buy the installation
	if (getLevel() != 0) 
		res = getValueAtLevel(getLevel()+1) - getCurrentValue();
	return res;
}

int Installation::getDowngradeRefunds(){
	// returns a positive value that represents the
	// funds that will be refunded when downgraded 1 level.
	int res = 0;
	if (getLevel() > 0)
		res = (getCurrentValue() - getValueAtLevel(getLevel()-1))*getRefundRatio();
	return res;
}

void Installation::upgrade(){
	if (getLevel() < getMaxLevel()){
		setLevel( getLevel() + 1 );
		setCurrentValue(getValueAtLevel(getLevel()));
	}
}

void Installation::downgrade(){
	if (getLevel() > 0){
		setLevel( getLevel() - 1 );
		setCurrentValue(getValueAtLevel(getLevel()));
	}
}

