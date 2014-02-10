#include "Broomstick.hpp"
#include "Gear.hpp"
#include "Player.hpp"

using namespace std;

Broomstick(const int const nCases, const int const velocity):cases(nCases), velocityBonus(velocity){

}

int Broomstick::getCases () const {
    return cases;
}

int Broomstick::getVelocityBonus () const{
    return velocityBonus;
}

