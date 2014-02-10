// Abstract class Installation. Methods to be implemented in subclasses : 
// 		- int getMaxLevel();

class Installation{
private:
	int _level;
	int _maintenanceCost;
	int _upgradeCost;
	int _downgradeRefunds;
public:

	// GETTERS + SETTERS
	int getLevel() { return _level; }
	void setLevel(int level) { _level = level; }

	int getMaintenanceCost() { return _maintenanceCost; }
	void setMaintenanceCost(int maintenanceCost) { _maintenanceCost = maintenanceCost; }

	int getUpgradeCost() { return _upgradeCost; }
	void setUpgradeCost(int upgradeCost) { _upgradeCost = upgradeCost; }

	int getDowngradeRefunds() { return _downgradeRefunds; }
	void setDowngradeRefunds(int downgradeRefunds) { _downgradeRefunds = downgradeRefunds; }

	// WORK METHODS
	void upgrade();
	void downgrade();

	virtual int getMaxLevel() = 0;
};