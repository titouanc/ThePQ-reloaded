#include "TrainingGroup.hpp"
#include "Player.hpp"
#include "Coach.hpp"

using namespace std;

TrainingGroup::TrainingGroup(){
	/*Constructor for the TrainingGroup object*/

}

void TrainingGroup::train(){
	/*Method*/

}

void TrainingGroup::AddCoach(Coach aCoach){
	/*Method for adding coach to group*/
}

void TrainingGroup::removeCoach(Coach aCoach){
	/*Method for removing coach from group*/

}

list<Coach> TrainingGroup::getCoaches(){
    //TODO Determine list type <<-see .hpp for options
}

void TrainingGroup::addPlayer(Player::aPlayer){
	/*Method for adding players to the group*/
}

void TrainingGroup::removePlayer (Player aPlayer){
	/*Method for removing players from the group*/

}

list<Player> getPlayers (){
    //TODO Determine list type
}

TrainingGroup::~TrainingGroup(){
	/*Destructor for the TrainingGroup object*/
}