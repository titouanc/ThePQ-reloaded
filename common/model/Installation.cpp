#include "Installation.hpp"
#include <cmath>

int Installation::getMaintenanceCost() const {
	/*Method returning integer representing the maintenance cost of the installation*/
	return getValueAtLevel(getLevel())/100; 
}

int Installation::getIncome() const {
	/*Method returning integer representing the income of the installation */
	return getValueAtLevel(getLevel())/50;
}

int Installation::getValueAtLevel(int level) const {
	/*Method returning an integer representing the value of the 
	*installation at the next level
	*/
	return level ? (getBaseValue() * pow(2, level+1)) : 0;
}

int Installation::getUpgradeCost() const{
	/*Method returning an integer representing the cost of an upgrade to
	 *the next level. 
	 */
	int res = getBaseValue();	// buy the installation
	if (getLevel() != 0) 
		res = getValueAtLevel(getLevel()+1) - getCurrentValue();
	return res;
}

int Installation::getDowngradeRefunds() const {
	/*Method returning an integer representing the
	 *funds that will be refunded when downgraded by 1 level. 
	 */
	int res = 0;
	if (getLevel() > 0)
		res = ((float)(getCurrentValue() - getValueAtLevel(getLevel()-1)))*getRefundRatio();
	return res;
}

void Installation::upgrade(){
	/*Method upgrading the installation (level and currentValue)*/
	if (getLevel() < getMaxLevel()){
		_level++;
	}
}

void Installation::downgrade(){
	/*Method downgrading the installation (level and currentValue)*/
	if (getLevel() > 0){
		_level--;
	}
}

FanShop::FanShop(std::string owner) : Installation(owner, "FanShop", 1000, 0)
{}

