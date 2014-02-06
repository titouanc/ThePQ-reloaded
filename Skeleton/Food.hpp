#ifndef Food_hpp
#define Food_hpp

class Food 
{
public:
    std::string getName ();
	int getPrice ();
	void setPrice (int price);
private:
    std::string name;
	int price;
};

#endif
