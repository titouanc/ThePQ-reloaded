#ifndef GearStore_hpp
#define GearStore_hpp

#include "Installation.hpp"
#include "Gear.hpp"

class GearStore : Installation 
{
public:
	void sell (Gear aGear,
	           Player aPlayer,
	           int quantity);
	list <Gear> getArticles ();
	void buy (Gear aGear,
	          Player from,
	          int quantity);
private:
	list <Gear> ArticlesList;
};

#endif;
