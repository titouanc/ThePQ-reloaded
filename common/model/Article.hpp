#ifndef Article_hpp
#define Article_hpp

class Article 
{
public:
	Article (std::string name, int price, std::string description);
    std::string getName () const;
	int getPrice () const;
    std::string getDescription () const;        
	void setPrice (int aPrice);
private:
    std::string const name;
	int price;
    std::string const description;
};

#endif
