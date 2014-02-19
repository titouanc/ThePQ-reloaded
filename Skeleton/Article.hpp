#ifndef __ARTICLE_HPP
#define __ARTICLE_HPP

class Article 
{
public:
	Article (std::string name, int price, \
		std::string description):	_name(name),_price(price),\
									_description(description){}
	
	/*Method returning the name of the article*/
    std::string getName () const 			{ return _name; }
	
	/*Method returning the price of the article*/
	int getPrice () const					{ return _price; }
    
    /*Method returning the description of the article*/
    std::string getDescription () const;	{ return _description; }   
	
	/*Method to set <<aPrice>> as the price of the article*/
	void setPrice (int aPrice);				{ _price = aPrice; }
	
private:
    std::string const _name;
	int _price;
    std::string const _description;
};

#endif
