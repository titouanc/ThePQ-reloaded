// Abstract class Installation. Methods to be implemented in subclasses : 
// 		- int getMaxLevel();

class Installation{
private:
	int _level;
	int _currentValue; // represents the price of the building at its current level.
	int _baseValue;
	float _refundRatio;
public:
	Installation():_level(0), _currentValue(100), _baseValue(100), _refundRatio(0.5) {} // TODO remove magic numbers?
	Installation(int level, int currentValue, float refundRatio=0.5) :
			_level(level), _currentValue(currentValue), 
			_baseValue(currentValue), _refundRatio(refundRatio){}

	// GETTERS + SETTERS
	int getLevel() { return _level; }
	void setLevel(int level) { if (level < getMaxLevel()) _level = level; }

	int getCurrentValue() { return _currentValue; }
	void setCurrentValue(int newValue) { _currentValue = newValue; }

	int getBaseValue() { return _baseValue; }
	void setBaseValue(int newValue) { _baseValue = newValue; }

	float getRefundRatio() { return _refundRatio; }
	void setRefundRatio(float refundRatio) { _refundRatio = refundRatio; } 

	// WORK METHODS
	int getMaintenanceCost();

	int getValueAtLevel(int level);

	int getUpgradeCost();	// returns a positive value that represents the cost 
							// of an upgrade to the next level.
	int getDowngradeRefunds();	// returns a positive value that represents the
								// funds that will be refunded when downgraded 1 level.
	
	void upgrade();
	void downgrade();

	// TODO this has to be pure virtual. 10 is just a value for tests.
	virtual int getMaxLevel() { return 10; }; 
};