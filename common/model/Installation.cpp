#include "Installation.hpp"
#include <cmath>
#include <iostream>

Installation* Installation::CAST(JSON::Dict const & json)
{
	std::string name = STR(json.get("name")).value();
	std::cout << name << std::endl;
	if (name == "Fan Shop") return new FanShop(json);
	else if (name == "Food Stand") return new FoodStand(json);
	else if (name == "Tribune") return new Tribune(json);
	else if (name == "Medical Center") return new MedicalCenter(json);
	// WHAT TODO ?
}

Installation::Installation(std::string owner, std::string name, int baseValue, int level, float refundRatio) :
_name(name), _level(level), _baseValue(baseValue), _refundRatio(refundRatio), _owner(owner){}

Installation::Installation(JSON::Dict const &json): Installation()
{
	if(ISSTR(json.get(memory::INST_TYPE)))		{_name = STR(json.get(memory::INST_TYPE)).value();}
	if(ISINT(json.get(memory::LEVEL)))			{_level = INT(json.get(memory::LEVEL));}
	if(ISINT(json.get(memory::BASE_VALUE))) 	{_baseValue = INT(json.get(memory::BASE_VALUE));}
	if(ISFLOAT(json.get(memory::REFUND_RATIO))){_refundRatio = FLOAT(json.get(memory::REFUND_RATIO));}
	if(ISSTR(json.get(net::MSG::USERNAME)))	{_owner = STR(json.get(net::MSG::USERNAME)).value();}
}

Installation::operator JSON::Dict()
{
	JSON::Dict json;
	json.set(memory::INST_TYPE, _name);
	json.set(memory::LEVEL, _level);
	json.set(memory::BASE_VALUE, _baseValue);
	json.set(memory::REFUND_RATIO, _refundRatio);
	json.set(net::MSG::USERNAME, _owner);
	return json;
}

Installation::~Installation()
{}

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

FanShop::FanShop(std::string owner) : Installation(owner, "Fan Shop", 1000, 0)
{}

