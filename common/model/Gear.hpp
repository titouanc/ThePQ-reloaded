#ifndef __GEAR_HPP
#define __GEAR_HPP

class Gear 
{
public:
    std::string getName () const ;
    std::string getDescription () const;
	int getPrice () const;
private:
    std::string const name;
    std::string const description;
	int const price;
};

#endif
