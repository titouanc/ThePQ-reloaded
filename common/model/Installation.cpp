#include "Installation.hpp"
#include <cmath>
#include <iostream>

Installation* Installation::CAST(JSON::Dict const & json)
{
	std::string name = STR(json.get("name")).value();
	if (name == memory::FANSHOP) return new FanShop(json);
	else if (name == memory::FOOD_STAND) return new FoodStand(json);
	else if (name == memory::TRIBUNE) return new Tribune(json);
	else if (name == memory::MEDIC_CENTER) return new MedicalCenter(json);
	// WHAT TODO ?
	return NULL;
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

int Installation::getCurrentValue() const { 
	return getValueAtLevel(_level);
}

int Installation::getValueAtLevel(int level) const {
	/*Method returning an integer representing the value of the 
	*installation at the next level
	*/
	return level ? (_baseValue * pow(2, level+1)) : 0;
}

int Installation::getUpgradeCost() const{
	/*Method returning an integer representing the cost of an upgrade to
	 *the next level. 
	 */
	int res = _baseValue;	// buy the installation
	if (_level > 0) 
		res = getValueAtLevel(_level+1) - getCurrentValue();
	return res;
}

int Installation::getDowngradeRefunds() const {
	/*Method returning an integer representing the
	 *funds that will be refunded when downgraded by 1 level. 
	 */
	int res = 0;
	if (_level > 0)
		res = ((float)(getCurrentValue() - getValueAtLevel(_level-1)))*_refundRatio;
	return res;
}

void Installation::upgrade(){
	/*Method upgrading the installation (level and currentValue)*/
	if (_level < getMaxLevel()){
		_level++;
	}
}

void Installation::downgrade(){
	/*Method downgrading the installation (level and currentValue)*/
	if (_level > 0){
		_level--;
	}
}

FanShop::FanShop(std::string owner) : Installation(owner, memory::FANSHOP, 1000)
{}

int FanShop::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/1000;
}

int FanShop::getIncome() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/50;
}
	
FoodStand::FoodStand(std::string owner) : Installation(owner, memory::FOOD_STAND, 500)
{}

int FoodStand::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/1000;	
}

int FoodStand::getIncome() const
{
	return 0;
}

Tribune::Tribune(std::string owner) : Installation(owner, memory::TRIBUNE, 2500, 1)
{}


int Tribune::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/1000;	
}

int Tribune::getIncome() const
{
	return 0;
}

MedicalCenter::MedicalCenter(std::string owner) : Installation(owner, memory::MEDIC_CENTER, 5000)
{}

int MedicalCenter::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/1000;	
}

int MedicalCenter::getIncome() const
{
	return 0;
}
