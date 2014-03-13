#ifndef __TRAININGINDIVIDUAL__.HPP
#define __TRAININGINDIVIDUAL__.HPP
#include "common/model/Player.hpp"
class TrainingIndividual
{
	private:
		Player _player;

	public:
		TrainingIndividual(Player player):_player(player){};
		~TrainingIndividual();

	/* data */
};
#endif // __TRAININGINDIVIDUAL__

TrainingIndividual::TrainingIndividual(Player &player){}

void TrainingIndividual::showCosts(){

}
void TrainingIndividual::checkBalance(){

}
void TrainingIndividual::train(){

}