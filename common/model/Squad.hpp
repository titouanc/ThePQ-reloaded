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
                case 2: chasers[i] = convertToChaser(toLoad);
                        break;
                case 3: ;
                case 4: beaters[i] = convertToBeater(toLoad,new Bat(0,0));
                        break;
                case 5: seeker = convertToSeeker(toLoad);
                        break;
                case 6: keeper = convertToKeeper(toLoad);
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

    Chaser convertToChaser(Player& player){
        Chaser chaser;
        chaser.setStrength(player.getStrength());
        chaser.setVelocity(player.getVelocity());
        chaser.setPrecision(player.getPrecision());
        chaser.setChance(player.getChance());
        chaser.equipBroomstick(player.getBroomstick());
        chaser.equipJersey(player.getJersey());
        chaser.setName(player.getName());
        chaser.setSalary(player.getSalary());
        chaser.setPrice(player.getSalary());
        chaser.setMemberID(player.getMemberID());
        chaser.setOwner(player.getOwner());
        return chaser;
    }

    Keeper convertToKeeper(Player& player){
        Keeper keeper;
        keeper.setStrength(player.getStrength());
        keeper.setVelocity(player.getVelocity());
        keeper.setPrecision(player.getPrecision());
        keeper.setChance(player.getChance());
        keeper.equipBroomstick(player.getBroomstick());
        keeper.equipJersey(player.getJersey());
        keeper.setName(player.getName());
        keeper.setSalary(player.getSalary());
        keeper.setPrice(player.getSalary());
        keeper.setMemberID(player.getMemberID());
        keeper.setOwner(player.getOwner());
        return keeper;
    }

    Seeker convertToSeeker(Player& player){
        Seeker seeker;
        seeker.setStrength(player.getStrength());
        seeker.setVelocity(player.getVelocity());
        seeker.setPrecision(player.getPrecision());
        seeker.setChance(player.getChance());
        seeker.equipBroomstick(player.getBroomstick());
        seeker.equipJersey(player.getJersey());
        seeker.setName(player.getName());
        seeker.setSalary(player.getSalary());
        seeker.setPrice(player.getSalary());
        seeker.setMemberID(player.getMemberID());
        seeker.setOwner(player.getOwner());
        return seeker;
    }

    Beater convertToBeater(Player& player, Bat bat){
        Beater beater;
        beater.setStrength(player.getStrength());
        beater.setVelocity(player.getVelocity());
        beater.setPrecision(player.getPrecision());
        beater.setChance(player.getChance());
        beater.equipBroomstick(player.getBroomstick());
        beater.equipJersey(player.getJersey());
        beater.equipBat(bat);
        beater.setName(player.getName());
        beater.setSalary(player.getSalary());
        beater.setPrice(player.getSalary());
        beater.setMemberID(player.getMemberID());
        beater.setOwner(player.getOwner());
        return beater;
    }
};

#endif
