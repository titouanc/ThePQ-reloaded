#include "Installation.hpp"
#include <cmath>

int Installation::getMaintenanceCost() const {
	return getValueAtLevel(getLevel())/10; 
}

int Installation::getValueAtLevel(int level) const {
	return level ? (getBaseValue() * pow(2, level+1)) : 0;
}

int Installation::getUpgradeCost() const{
	/* returns a positive value that represents the cost of an upgrade to
	 * the next level. 
	 */
	int res = getBaseValue();	// buy the installation
	if (getLevel() != 0) 
		res = getValueAtLevel(getLevel()+1) - getCurrentValue();
	return res;
}

int Installation::getDowngradeRefunds() const {
	/* returns a positive value that represents the
	 * funds that will be refunded when downgraded 1 level. 
	 */
	int res = 0;
	if (getLevel() > 0)
		res = getCurrentValue() - getValueAtLevel(getLevel()-1))*getRefundRatio();
	return res;
}

void Installation::upgrade(){
	/* Upgrades the installation (its level, and its currentValue)
	 */
	if (getLevel() < getMaxLevel()){
		setLevel( getLevel() + 1 );
	}
}

void Installation::downgrade(){
	/* Downgrades the installation (its level, and its currentValue)
	 */
	if (getLevel() > 0){
		setLevel( getLevel() - 1 );
	}
}

