#include "Food.hpp"

using namespace std;

string Food::getName () const{
    return name;
}

int Food::getPrice () const{
    return price;
}

void Food::setPrice (int newPrice){
    price = newPrice;
}
