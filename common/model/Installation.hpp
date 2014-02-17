#ifndef __INSTALLATION_HPP
#define __INSTALLATION_HPP

// Abstract class Installation. Methods to be implemented in subclasses : 
// 		- int getMaxLevel();

#include <json/json.hpp>
#include <string>

class Installation{
private:
	std::string _name;
	int _level;
	int _baseValue;
	float _refundRatio;
public:
	Installation(JSON::Dict json) : _name(STR(json.get("name")).value()),
									   _level(INT(json.get("level"))), 
									   _baseValue(INT(json.get("baseValue"))),
									   _refundRatio(FLOAT(json.get("refundRatio")))
	{}
	
	operator JSON::Dict()
	{
		JSON::Dict json;
		json.set("name", _name);
		json.set("level", _level);
		json.set("baseValue", _baseValue);
		json.set("refundRatio", _refundRatio);
		return json;
	}
	
	Installation():_name(""), _level(0), _baseValue(100), _refundRatio(0.5) {} 
	Installation(std::string name, int baseValue, int level, float refundRatio=0.5) :
			_name(name),
			_level(level), 
			_baseValue(baseValue), _refundRatio(refundRatio){}

	// GETTERS + SETTERS
	int getLevel() const { return _level; }
	void setLevel(int level) { if (level < getMaxLevel()) _level = level; }

	int getCurrentValue() const { return getValueAtLevel(getLevel()); }

	int getBaseValue() const { return _baseValue; }
	void setBaseValue(int newValue) { _baseValue = newValue; }

	float getRefundRatio() const { return _refundRatio; }
	void setRefundRatio(float refundRatio) { _refundRatio = refundRatio; } 
	
	std::string getName() const { return _name; }
	void setName(std::string name) { _name = name; }
	// -----------------

	int getMaintenanceCost() const ;

	int getValueAtLevel(int level) const ;

	int getUpgradeCost() const ;	// returns a positive value that represents the cost 
							// of an upgrade to the next level.
	int getDowngradeRefunds() const ;	// returns a positive value that represents the
								// funds that will be refunded when downgraded 1 level.
	
	void upgrade();
	void downgrade();

	// TODO this has to be pure virtual. 10 is just a value for tests.
	virtual int getMaxLevel() { return 10; }; 
};

#endif
