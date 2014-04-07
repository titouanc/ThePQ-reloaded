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
	Player* players[7];
    Jersey _jersey;
    Broomstick _broomstick;
    Bat _bat;

    Squad() : _jersey(), _broomstick(), _bat() { setVectorOfPlayers(); }

    Squad(Jersey& jersey, Broomstick& broom, Bat& bat) : Squad() {
        _jersey = jersey; _broomstick = broom; _bat = bat;
    }

	Squad(JSON::Dict const & json) :  Squad() {
        /* ID - owner */
		if (ISINT(json.get("squad_id")))
			squad_id = INT(json.get("squad_id"));
        if (ISSTR(json.get("squad_owner")))
            squad_owner = STR(json.get("squad_owner")).value();

        /* Chasers */
		if (ISLIST(json.get("chasers")) && LIST(json.get("chasers")).len() == 3){
			JSON::List const & list = LIST(json.get("chasers"));
			for (int i=0; i<3; i++)
				chasers[i] = Chaser(DICT(list[i]));
		}

        /* Beaters */
		if (ISLIST(json.get("beaters")) && LIST(json.get("beaters")).len() == 2){
			JSON::List const & list = LIST(json.get("beaters"));
			for (int i=0; i<2; i++)
				beaters[i] = Beater(DICT(list[i]));
		}

        /* Seeker */
		if (ISDICT(json.get("seeker")))
			seeker = Seeker(DICT(json.get("seeker")));

        /* Keeper */
		if (ISDICT(json.get("keeper")))
			keeper = Keeper(DICT(json.get("keeper")));
		
        /* Gears */
        if(ISDICT(json.get("broomstick")))
            _broomstick = Broomstick(DICT(json.get("broomstick")));
        if(ISDICT(json.get("jersey")))
            _jersey = Jersey(DICT(json.get("jersey")));
        if(ISDICT(json.get("bat")))
            _bat = Bat(DICT(json.get("bat")));

        setVectorOfPlayers();
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
        _broomstick = other._broomstick;
        _bat = other._bat;
        _jersey = other._jersey;
        setVectorOfPlayers();
        return *this;
    }

    operator JSON::Dict() const {
        JSON::Dict res;
        res.set("squad_id", squad_id);
        res.set("squad_owner", squad_owner);
        res.set("seeker", JSON::Dict(seeker));
        res.set("keeper", JSON::Dict(keeper));
        res.set("jersey",JSON::Dict(_jersey));
        res.set("bat",JSON::Dict(_bat));
        res.set("broomstick",JSON::Dict(_broomstick));
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

    /** Gives address of each beater, chaser, keeper and seeker to players[7] */
    void setVectorOfPlayers(){
        size_t i = 0;
        for(size_t j = 0; j < 3; ++j){
            players[i] = &chasers[j];
            ++i;
        }
        for(size_t j = 0; j < 2; ++j){
            players[i] = &beaters[j];
            ++i;
        }
        players[i] = &seeker; ++i;
        players[i] = &keeper; ++i; 
    }


    bool hasPlayer(Moveable *moveable) const{
        for (int i=0; i<7; i++)
            if (players[i] == moveable)
                return true;
        return false;
    }

    bool hasMemberID(int member_id) const {
        for (int i=0; i<7; i++)
            if (players[i]->getMemberID() == member_id)
                return true;
        return false;
    }

    void putPlayerAtPosition(int member_id, int position){
        Player player(member_id, squad_owner);
        MemoryAccess::load(player);
        if (position < 3) {                     // Chaser
            chasers[position] = player;
            equipAll(&(chasers[position]));
        }
        else if (position > 2 && position < 5){  // Beater
            beaters[position-3] = player;
            equipAll(&(beaters[position-3]));
        }
        else if (position == 5){                // Seeker
            seeker = player;
            equipAll(&seeker);
        }
        else if (position == 6){                // keeper
            keeper = player;
            equipAll(&keeper);
        }
    }

    void swapPlayers(int member_id, int position){
        int player2Pos;
        for (int i=0; i<7; i++)
            if (players[i]->getMemberID() == member_id)
                player2Pos = i;
        putPlayerAtPosition(players[position]->getMemberID(), player2Pos);
        putPlayerAtPosition(member_id, position);
    }

    void equipAll(Player* player){
        player->equipBroomstick(&_broomstick);
        player->equipJersey(&_jersey);
        if(player->isBeater()){
            player->equipBat(&_bat);
        }
    }

    // Chaser convertToChaser(Player& player, Jersey& jersey, Broomstick& broom){
    //     Chaser chaser;
    //     chaser.setStrength(player.getStrength());
    //     chaser.setVelocity(player.getVelocity());
    //     chaser.setPrecision(player.getPrecision());
    //     chaser.setChance(player.getChance());
    //     chaser.equipBroomstick(broom);
    //     chaser.equipJersey(jersey);
    //     chaser.setName(player.getName());
    //     chaser.setSalary(player.getSalary());
    //     chaser.setPrice(player.getSalary());
    //     chaser.setMemberID(player.getMemberID());
    //     chaser.setOwner(player.getOwner());
    //     return chaser;
    // }

    // Keeper convertToKeeper(Player& player, Jersey& jersey, Broomstick& broom){
    //     Keeper keeper;
    //     keeper.setStrength(player.getStrength());
    //     keeper.setVelocity(player.getVelocity());
    //     keeper.setPrecision(player.getPrecision());
    //     keeper.setChance(player.getChance());
    //     keeper.equipBroomstick(broom);
    //     keeper.equipJersey(jersey);
    //     keeper.setName(player.getName());
    //     keeper.setSalary(player.getSalary());
    //     keeper.setPrice(player.getSalary());
    //     keeper.setMemberID(player.getMemberID());
    //     keeper.setOwner(player.getOwner());
    //     return keeper;
    // }

    // Seeker convertToSeeker(Player& player, Jersey& jersey, Broomstick& broom){
    //     Seeker seeker;
    //     seeker.setStrength(player.getStrength());
    //     seeker.setVelocity(player.getVelocity());
    //     seeker.setPrecision(player.getPrecision());
    //     seeker.setChance(player.getChance());
    //     seeker.equipBroomstick(broom);
    //     seeker.equipJersey(jersey);
    //     seeker.setName(player.getName());
    //     seeker.setSalary(player.getSalary());
    //     seeker.setPrice(player.getSalary());
    //     seeker.setMemberID(player.getMemberID());
    //     seeker.setOwner(player.getOwner());
    //     return seeker;
    // }

    // Beater convertToBeater(Player& player, Jersey& jersey, Broomstick& broom, Bat& bat){
    //     Beater beater;
    //     beater.setStrength(player.getStrength());
    //     beater.setVelocity(player.getVelocity());
    //     beater.setPrecision(player.getPrecision());
    //     beater.setChance(player.getChance());
    //     beater.equipBroomstick(broom);
    //     beater.equipJersey(jersey);
    //     beater.equipBat(bat);
    //     beater.setName(player.getName());
    //     beater.setSalary(player.getSalary());
    //     beater.setPrice(player.getSalary());
    //     beater.setMemberID(player.getMemberID());
    //     beater.setOwner(player.getOwner());
    //     return beater;
    // }
};

#endif
