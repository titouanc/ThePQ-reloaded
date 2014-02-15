#include "Chaser.hpp"
#include "Player.hpp"

bool Chaser::isChaser()const {
    return true;
}

int Chaser::speed (){
    return ( (float) velocity * .85 + (float) strength * .1 + (float) chance * .05 ) * 100;
}

int Chaser::collisionner (){
    return ( (float) strength * .65 + (float) magic * .3 + (float) chance * .05 ) * 100;
}

int Chaser::anticollision (){
    return ( (float) constitution * .65 + (float) spirit * .3 + (float) chance * .05 ) * 100 ;
}

int Chaser::pass (){
    return ( (float) strength * .4 + (float) precision * .5 + (float) chance * 0.1 ) * 100 ;
}

int Chaser::shoot (){
    return ( (float) strength * .6 + (float) precision * .35 + (float) chance * .05 ) *100 ;
}
