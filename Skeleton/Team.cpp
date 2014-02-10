#include "Team_hpp"
#include "Gear.hpp"
#include "Sponsor.hpp"
#include "Installation.hpp"

using namespace std;

float Team::getRatio (){
    return nbrWin/nbrPlayed;
}

void Team::giveMatchRes (MatchResult result){

}

list<Gear> Team::getAvailableEquipment (){
    //TODO Determine list type
}

list<Gear> Team::equipments{
    //TODO Determine list type
}
