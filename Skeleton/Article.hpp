#ifndef Article_hpp
#define Article_hpp

class Article 
{
public:
	Article (std::string name, int price, std::string description);
    std::string getName ();
	int getPrice ();
    std::string getDescription ();        
	void setPrice (int price);
private:
    std::string name;
	int price;
    std::string description;
};

#endif
