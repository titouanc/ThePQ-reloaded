#ifndef __FOOD_HPP
#define __FOOD_HPP

class Food 
{
public:
    std::string getName () const ;
	int getPrice () const ;
	void setPrice (int newPrice);
private:
    std::string const name;
	int price;
};

#endif
