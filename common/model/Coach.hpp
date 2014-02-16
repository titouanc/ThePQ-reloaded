#ifndef __COACH_HPP
#define __COACH_HPP

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
