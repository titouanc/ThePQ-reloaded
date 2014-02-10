#ifndef GearStore_hpp
#define GearStore_hpp

#include "Installation.hpp"
#include "Gear.hpp"

class GearStore : Installation 
{
public:
	void sell (Gear aGear,
	           Team aTeam,
	           int quantity);
	list <Gear> getArticles () const;
	void buy (Gear aGear,
	          Team aTeam,
	          int quantity);
private:
	list <Gear> articlesList;
};

#endif;
