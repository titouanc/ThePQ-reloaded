#ifndef Abilities_hpp
#define Abilities_hpp

class Abilities 
{
public:
	Abilities ();
	int getStrength ();
	int getConstitution ();
	int getMagic ();
	int getSpirit ();
	int getVelocity ();
	int getPrecision ();
	int getChance ();
	void improveStrength (int added);
	void improveConstitution (int added);
	void improveMagic (int added);
	void improveSpirit (int added);
	void improveVelocity (int added);
	void improvePrecision (int added);
private:
	int strength;
	int constitution;
	int magic;
	int spirit;
	int velocity;
	int precision;
	int chance;
};

#endif
