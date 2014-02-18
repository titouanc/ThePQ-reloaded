#include "Championship.hpp"
#include "ParticipatingTeam.hpp"
#include "Schedule.hpp"
#include "Ranking.hpp"

using namespace std;
Championship::Championship(){
	/*Constructor of object Championship.
	 *It creates the championship generates and
	 *makes available the schedule the participating teams
	 *and the final ranking order
	 */
}

ParticipatingTeam[] Championship::getTeams (){
	/*Method returning the participating funcions
	 *in the championship
	 */
}

void Championship::startChampionship (){
	/*Method launching a championship*/
}

void Championship::addTeam (ParticipatingTeam team){
	/*Method for adding <<team>> to the championship*/
}

void Championship::deleteTeam (ParticipatingTeam team){
	/*Method for removing team from championship*/
}

Schedule Championship::getSchedule (ParticipatingTeam team){
	/*Method returning the schedule of team*/
}

Ranking Championship::getRanking (){
	/*Method returning the ranking of the championship*/
}
