#ifndef Food_hpp
#define Food_hpp

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
