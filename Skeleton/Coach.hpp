#ifndef Coach_hpp
#define Coach_hpp

#include "Member.hpp"

class Coach : Member 
{
public:
	Coach ();
	void Train (Player aPlayer);
private:
	int stregthTrainer;
	int ConstitutionTrainer;
	int magicTrainer;
	int spiritTrainer;
	int velocityTrainer;
	int precisionTrainer;
};

#endif
