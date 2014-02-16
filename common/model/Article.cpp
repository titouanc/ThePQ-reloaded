#include "Article.hpp"

using namespace std;

Article::Article (string name, int price, string description):name(name),price(price),description(description){

}

string Article::getName () const{
    return name;
}

int getPrice () const{
    return price;
}

string Article::getDescription () const{
    return description;
}        

void Article::setPrice (int aPrice){
    price = aPrice;
}

