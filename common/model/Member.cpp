#include "Member.hpp"
#include "Team.hpp"

using namespace std;

Member::Member():name("Lasty Cotter"), salary(5000), price(25000), owner(new Team()){

}

string Member::getName(){
    return name;
}

int Member::getSalary(){
    return salary;
}

int Member::getPrice(){
    return price;
}

Team Member::getOwner(){
    return *owner;
}

void Member::setOwner(Team aTeam){
    *owner = aTeam;
}
