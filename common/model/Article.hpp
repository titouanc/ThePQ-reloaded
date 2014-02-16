#ifndef __ARTICLE_HPP
#define __ARTICLE_HPP

class Article 
{
public:
	Article (std::string name, int price, std::string description):	_name(name),
																	_price(price),
																	_description(description){}
    std::string getName () const 			{ return _name; }
	int getPrice () const					{ return _price; }
    std::string getDescription () const;	{ return _description; }   
	void setPrice (int aPrice);				{ _price = aPrice; }
private:
    std::string const _name;
	int _price;
    std::string const _description;
};

#endif
