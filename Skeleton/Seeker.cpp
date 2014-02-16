#include "Seeker.hpp"
#include "Player.hpp"

Seeker::Seeker() : Player(){

}

bool Seeker::isSeeker() const {
    return true;
}

int Seeker::catchGS (){
    return static_cast<int>( (float) precision * .50 + (float) velocity * .20 + (float) strength * .10 + (float) magic * .1 + (float) chance * .1 ) * 100 ;
}

