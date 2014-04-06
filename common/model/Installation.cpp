#include "Installation.hpp"
#include <cmath>
#include <iostream>

Installation* Installation::CAST(JSON::Dict const & json)
{
	std::string name = STR(json.get("name")).value();
	if (name == memory::FANSHOP) return new FanShop(json);
	else if (name == memory::FOOD_STAND) return new FoodStand(json);
	else if (name == memory::TRIBUNE) return new Tribune(json);
	return NULL;
}

/// Constructor
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

/// Destructor
Installation::~Installation()
{}

int Installation::getCurrentValue() const { 
	return getValueAtLevel(_level);
}

/** 
 * Method returning an integer representing the value of the 
 * installation at the next level
 * @param int : level at which to get the cost level
 * @return int : value at level
 */
int Installation::getValueAtLevel(int level) const {
	return level ? (_baseValue * pow(2, level+1)) : 0;
}

/**
 * Method returning an integer representing the cost of an upgrade to
 * the next level. 
 * @return int : cost of the upgrade
 */
int Installation::getUpgradeCost() const{
	return getValueAtLevel(_level+1) - getCurrentValue();
}

/**
 * Method returning an integer representing the cost of a downgrade to
 * the previous level. 
 * @return int : cost of the downgrade
 */
int Installation::getDowngradeRefunds() const {
	int res = 0;
	if (_level > 0)
		res = ((float)(getCurrentValue() - getValueAtLevel(_level-1)))*_refundRatio;
	return res;
}

/**
 * Method upgrading the installation (level and currentValue)
 */
void Installation::upgrade(){
	
	if (_level < getMaxLevel()){
		_level++;
	}
}

/** 
 *Method downgrading the installation (level and currentValue)
 */
void Installation::downgrade(){

	if (_level > 0){
		_level--;
	}
}
/// Constructor
FanShop::FanShop(std::string owner) : Installation(owner, memory::FANSHOP, 10000)
{}

/**
 * Method handling the maintenance cost of an installation
 * @return int : cost of maintanance for the installation
 */
int FanShop::getMaintenanceCost() const
{
	return getCurrentValue()/50;
}

/**
 * Method handling the income from an installation
 * @return int : income of installation
 */
int FanShop::getIncome() const
{
	return (getCurrentValue()*gameconfig::TICKS_BEFORE_STADIUM)/(50*gameconfig::TICKS_BEFORE_RESET); // CAN'T TOUCH THIS MC HAMMER
}

/**
 * Method handling the income from a match
 * @return int : income from a match
 */
int FanShop::getMatchIncome(int nbOfSpectators)
{
	return nbOfSpectators*gameconfig::GOODIES_PRICE;
}
	
/// Constructor
FoodStand::FoodStand(std::string owner) : Installation(owner, memory::FOOD_STAND, 10000)
{}

/**
 * Method handling the maintenance cost
 * @return int : maintenance cost for the food stand
 */
int FoodStand::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/50;	
}

/**
 * Method handling the income from the food stand
 * @return int : income from the food stand
 */
int FoodStand::getIncome() const
{
	return 0;
}

/**
 * Method handling the incom from a match
 * @return int : income of a food stand after a match
 */
int FoodStand::getMatchIncome(int nbOfSpectators)
{
	return nbOfSpectators*gameconfig::FOOD_PRICE;
}

/// Constructor
Tribune::Tribune(std::string owner) : Installation(owner, memory::TRIBUNE, 20000, 1)
{}

/**
 * Method
 */
int Tribune::getMaxSpectators()
{
	return _level*gameconfig::TRIBUNE_SPECTATORS;
}

/**
 * Method handling the maintenance of a tribune
 * @return int : maintenance cost for a tribune
 */
int Tribune::getMaintenanceCost() const
{
	// EXAMPLE ==> TODO
	return getCurrentValue()/50;	
}

/**
 * Method handling the income of the tribune
 * @return int : income for a tribune
 */
int Tribune::getIncome() const
{
	return 0;
}

/**
 * Method handling the match income for a tribune
 * @return int : income for a tribune after a match
 */
int Tribune::getMatchIncome(int nbOfSpectators)
{
	return nbOfSpectators*gameconfig::TICKET_PRICE;
}