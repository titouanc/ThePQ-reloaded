#ifndef __INSTALLATION_HPP
#define __INSTALLATION_HPP

/*=======Abstract class Installation. Methods to be implemented in subclasses : 
 		- int getMaxLevel();
 		- int getMaintenanceCost();
 		- int getIncome();
		- int getIncome(Match&);
 		* // TODO balancing etc...
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
	static Installation* CAST(JSON::Dict const & json);
	Installation(std::string owner = "", std::string name = "", int baseValue = 100, 
					int level = 0, float refundRatio=0.5);
	Installation(JSON::Dict const &json);
	operator JSON::Dict();
	virtual ~Installation();
	
	/*=========Getters==========*/
	std::string getOwner() const {return _owner;}
	int getLevel() const { return _level; }
	int getBaseValue() const { return _baseValue; }
	float getRefundRatio() const { return _refundRatio; }	
	std::string getName() const { return _name; }
	
	int getCurrentValue() const;
	int getValueAtLevel(int level) const; // TODO on peut la mettre virtuelle pure eventuellement...
	int getUpgradeCost() const;	// returns a positive value that represents the cost 
							// of an upgrade to the next level.
	int getDowngradeRefunds() const;	// returns a positive value that represents the
								// funds that will be refunded when downgraded 1 level.
	
	/*=========Setters===========*/
	void setOwner(std::string username) {_owner = username;}
	void upgrade();
	void downgrade();
	
	/*==========Virtual==========*/
	virtual int getMaintenanceCost() const = 0;
	virtual int getIncome() const = 0;
	virtual int getMaxLevel() = 0;
	virtual int getMatchIncome(int nbOfSpectators) = 0;
};

class FanShop : public Installation
{
public:
	FanShop(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaintenanceCost() const;
	virtual int getIncome() const;
	virtual int getMaxLevel() { return 10; }
	virtual int getMatchIncome(int nbOfSpectators);

};

class FoodStand : public Installation
{
public:
	FoodStand(std::string owner = "");
	using Installation::Installation;
	
	virtual int getMaintenanceCost() const;
	virtual int getIncome() const;
	virtual int getMaxLevel() { return 10; }
	virtual int getMatchIncome(int nbOfSpectators);

};
class Tribune : public Installation
{
public:
	Tribune(std::string owner = "");
	using Installation::Installation;
	
	int getMaxSpectators();

	virtual int getMaintenanceCost() const;
	virtual int getIncome() const;
	virtual int getMaxLevel() { return 10; }
	virtual int getMatchIncome(int nbOfSpectators);

};

#endif
