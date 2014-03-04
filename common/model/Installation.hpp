#ifndef __INSTALLATION_HPP
#define __INSTALLATION_HPP

/*=======Abstract class Installation. Methods to be implemented in subclasses : 
 		- int getMaxLevel();
*/

#include <json/json.hpp>
#include <string>
#include <Constants.hpp>

class Installation
{
protected:
	std::string _name;
	int _level;
	int _baseValue;
	float _refundRatio;
	std::string _owner;
public:
	Installation(std::string owner = "", std::string name = "", int baseValue = 100, int level = 0, float refundRatio=0.5) :
			_name(name), _level(level), _baseValue(baseValue), _refundRatio(refundRatio), _owner(owner){}

	Installation(JSON::Dict const &json): Installation(){
		if(json.hasKey(memory::INST_TYPE)) 		{_name = STR(json.get(memory::INST_TYPE)).value();}
		if(json.hasKey(memory::LEVEL))			{_level = INT(json.get(memory::LEVEL));}
		if(json.hasKey(memory::BASE_VALUE)) 	{_baseValue = INT(json.get(memory::BASE_VALUE));}
		if(json.hasKey(memory::REFUND_RATIO))	{_refundRatio = FLOAT(json.get(memory::REFUND_RATIO));}
		if(json.hasKey(net::MSG::USERNAME))		{_owner = STR(json.get(net::MSG::USERNAME)).value();}
	}

	
	operator JSON::Dict()
	{
		JSON::Dict json;
		json.set(memory::INST_TYPE, _name);
		json.set(memory::LEVEL, _level);
		json.set(memory::BASE_VALUE, _baseValue);
		json.set(memory::REFUND_RATIO, _refundRatio);
		json.set(net::MSG::USERNAME, _owner);
		return json;
	}
	
	virtual ~Installation() {}
	
	static Installation* CAST(JSON::Dict const & json);
	
	/*=========Getters==========*/
	std::string getOwner() const {return _owner;}
	int getLevel() const { return _level; }
	int getBaseValue() const { return _baseValue; }
	float getRefundRatio() const { return _refundRatio; }	
	std::string getName() const { return _name; }
	
	/*=========Setters===========*/
	void setOwner(std::string username) {_owner = username;}
	void setRefundRatio(float refundRatio) { _refundRatio = refundRatio; } 
	void setName(std::string name) { _name = name; }

	/*****************************/
	void upgrade();
	void downgrade();
	/*****************************/
	int getCurrentValue() const { return getValueAtLevel(getLevel()); }
	int getMaintenanceCost() const ;
	int getIncome() const;
	int getValueAtLevel(int level) const ;
	int getUpgradeCost() const ;	// returns a positive value that represents the cost 
							// of an upgrade to the next level.
	int getDowngradeRefunds() const ;	// returns a positive value that represents the
								// funds that will be refunded when downgraded 1 level.

	// TODO this has to be pure virtual. 10 is just a value for tests.
	virtual int getMaxLevel() = 0;
};

class FanShop : public Installation
{
public:
	FanShop(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaxLevel() { return 10; }

};

class FoodStand : public Installation
{
public:
	FoodStand(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaxLevel() { return 10; }

};
class Tribune : public Installation
{
public:
	Tribune(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaxLevel() { return 10; }

};
class MedicalCenter : public Installation
{
public:
	MedicalCenter(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaxLevel() { return 10; }

};

#endif
