// Abstract class Installation. Methods to be implemented in subclasses : 
// 		- int getMaxLevel();

class Installation{
private:
	int _level;
	int _currentValue; // represents the price of the building at its current level.
public:
	Installation():_level(0), _currentValue(100) {} // TODO remove magic number
	Installation(int level, int currentValue) : _level(level), _currentValue(currentValue){}
	
	// GETTERS + SETTERS
	int getLevel() { return _level; }
	void setLevel(int level) { _level = level; }

	int getCurrentValue() { return _currentValue; }
	void setCurrentValue(int newValue) { _currentValue = newValue; }

	// WORK METHODS
	int getMaintenanceCost();

	int getUpgradeCost();
	int getDowngradeRefunds();
	
	void upgrade();
	void downgrade();

	// TODO this has to be pure virtual. 10 is just a value for tests.
	virtual int getMaxLevel() { return 10; }; 
};