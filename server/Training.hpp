#ifndef _TRAINING_HPP
#define _TRAINING_HPP
#include "TrainingGroup.hpp"
#include "Coach.hpp"
#include "Player.hpp"


enum trainingType={'Agility','Precision','Endurance'}//<<=add more trainings

class Training{
private:
	list<TrainingGroup> groups;
	size_t counter=0;//ensure max number of groups

public:
	Training();
	addGroup(TrainingGroup aGroup);
	removeGroup(TrainingGroup aGroup);
	selectTraining(trainingType aTraining);
	selectDuration(size_t time,TrainingGroup aGroup);
	~Training();

};
#endif