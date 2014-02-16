#ifndef Gear_hpp
#define Gear_hpp

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
