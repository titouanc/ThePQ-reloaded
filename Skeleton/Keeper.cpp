#include "Keeper.hpp"
#include "Player.hpp"

Keeper::Keeper() : Player(){

}

bool Keeper::isKeeper(){
    return true;
}

int Keeper::catchBall (){
    return static_cast<int>( (float) velocity * .65 + (float) precision * .3 + (float) chance * .05 ) * 100;
}

int Keeper::pass (){
    return static_cast<int>( (float) strength * .65 + (float) precision * .25 + (float) chance * .1) * 100 ;
}

