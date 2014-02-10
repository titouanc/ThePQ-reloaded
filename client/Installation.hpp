// Abstract class Installation. Methods to be implemented in subclasses : 
// 		- int getMaxLevel();

class Installation{
private:
	int _level;
	int _currentValue; // represents the price of the building at its current level.
public:

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

	virtual int getMaxLevel() = 0;
};