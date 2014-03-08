#ifndef DEFINE_SQUAD_HEADER
#define DEFINE_SQUAD_HEADER

#include <cstdlib>
#include <string>
#include "Player.hpp"
#include <json/json.hpp>
#include "MemoryAccess.hpp"

struct Squad {
	int squad_id;
	int client_id;
	std::string squad_owner;
	Chaser chasers[3];
	Beater beaters[2];
	Seeker seeker;
	Keeper keeper;
	Player *players[7];

	Squad(){
		for (int i=0; i<3; i++)
			players[i] = &(chasers[i]);
		for (int i=0; i<2; i++)
			players[i+3] = &(beaters[i]);
		players[5] = &seeker;
		players[6] = &keeper;
	}
    /*
    Squad(string name, int listID [7]){
        std::vector<Player> toLoad;
        MemoryAccess::load(toLoad, name);
        bool found;
        size_t i(0);
        for (i ; i < 3 ; ++i){
            found = false ;
            for (std::vector<Player>::iterator it(toLoad.begin()); ( found || it != toLoad.end()) ; ++it){
                if (it->getMemberID() == listID[i]){
                    players[i] = new Chaser(*it);
                    found = true ;
                }
            }
        }
        for (i ; i < 5 ; ++i){
            found = false ;
            for (std::vector<Player>::iterator it(toLoad.begin()); ( found || it != toLoad.end()) ; ++it){
                if (it->getMemberID() == listID[i]){
                    players[i] = new Beater(*it);
                    found = true ;
                }
            }
        }
        found = false ;
        for (std::vector<Player>::iterator it(toLoad.begin()); ( found || it != toLoad.end()) ; ++it){
            if (it->getMemberID() == listID[i]){
                players[i] = new Seeker(*it);
                found = true ;
            }
        }
        ++i;
        for (std::vector<Player>::iterator it(toLoad.begin()); ( found || it != toLoad.end()) ; ++it){
            if (it->getMemberID() == listID[i]){
                players[i] = new Keeper(*it);
                found = true ;
            }
        }

    }*/

    Squad(std::string name, int listID[7]){
        squad_owner = name;
        size_t i(0);
        for (i;i<7;++i){
            Player toLoad(listID[i], squad_owner);
            MemoryAccess::load(toLoad);
            switch (i){
                case 0: ;
                case 1: ;
                case 2: chasers[i] = new Chaser(toLoad);
                        break;
                case 3: ;
                case 4: beaters[i] = new Beater(toLoad,new Bat(0,0));
                        break;
                case 5: seeker = new Seeker(toLoad);
                        break;
                case 6: keeper = new Keeper(toLoad);
                        break;
            }
        }
        Squad();
    }

	Squad(JSON::Dict const & json) :  Squad() {
		if (ISINT(json.get("squad_id")))
			squad_id = INT(json.get("squad_id"));
		if (ISLIST(json.get("chasers")) && LIST(json.get("chasers")).len() == 3){
			JSON::List const & list = LIST(json.get("chasers"));
			for (int i=0; i<3; i++)
				chasers[i] = Chaser(DICT(list[i]));
		}
		if (ISLIST(json.get("beaters")) && LIST(json.get("beaters")).len() == 2){
			JSON::List const & list = LIST(json.get("beaters"));
			for (int i=0; i<2; i++)
				beaters[i] = Beater(DICT(list[i]));
		}
		if (ISDICT(json.get("seeker")))
			seeker = Seeker(DICT(json.get("seeker")));
		if (ISDICT(json.get("keeper")))
			keeper = Keeper(DICT(json.get("keeper")));
		if (ISSTR(json.get("squad_owner")))
			squad_owner = STR(json.get("squad_owner")).value();
	}
	Squad & operator=(Squad const & other){
		squad_id = other.squad_id;
		client_id = other.client_id;
		int i;
		for (i=0; i<3; i++) chasers[i] = other.chasers[i];
		for (i=0; i<2; i++) beaters[i] = other.beaters[i];
		seeker = other.seeker;
		keeper = other.keeper;
		squad_owner = other.squad_owner;
		return *this;
	}
	operator JSON::Dict(){
		JSON::Dict res;
		res.set("squad_id", squad_id);
		res.set("squad_owner", squad_owner);
		res.set("seeker", JSON::Dict(seeker));
		res.set("keeper", JSON::Dict(keeper));

		JSON::List b;
		for (int i=0; i<2; i++)
			b.append(JSON::Dict(beaters[i]));
		res.set("beaters", b);

		JSON::List c;
		for (int i=0; i<3; i++)
			c.append(JSON::Dict(chasers[i]));
		res.set("chasers", c);
		return res;
	}
};

#endif
