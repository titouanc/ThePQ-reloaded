#include "Player.hpp"
#include <model/MemoryAccess.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>

std::string paddName(std::string name){
    std::string newName="";
    if (name.length()>35) return name;
    else{
        newName+=name;
        for(unsigned int i(0);i<35-name.length();i++){
            newName+=" ";
        }
        return newName;
    }
}

std::string paddValue(int value){
    if (value < 10)
        return std::to_string(value)+"  ";
    else if (value<100)
        return std::to_string(value)+" ";
    else return std::to_string(value);
}

std::ostream& operator<< (std::ostream& out, const Player& player)
    {
        out << "        "
            << "\033[35m#" << player._memberID << "\033[0m "
            << paddName(player._name) 
            << "Strength(\033[33m" << paddValue(player._strength)
            << "\033[0m) Velocity(\033[33m" << paddValue(player._velocity) 
            << "\033[0m) Precision(\033[33m" << paddValue(player._precision)
            << "\033[0m) Chance (\033[33m" << paddValue(player._chance) << "\033[0m)";
        return out;
    }

Player::Player(JSON::Dict const & json) : 
    Member(json), Moveable(json), 
     _broomstick(NULL), _jersey(NULL), 
     _strength(5), _velocity(5), _precision(5), _chance(5) {

	//if (ISINT(json.get("memberID")))     _memberID = INT(json.get("memberID"));
	if (ISSTR(json.get("name")))         _name = STR(json.get("name")).value();
    if (ISINT(json.get("strength")))     _strength = INT(json.get("strength"));
    if (ISINT(json.get("velocity")))     _velocity = INT(json.get("velocity"));
    if (ISINT(json.get("precision")))    _precision = INT(json.get("precision"));
    if (ISINT(json.get("chance")))       _chance = INT(json.get("chance"));
}

Player::Player(const Player & player) : 
    Member(player._memberID,player._name,player._salary,player._price,player._owner), 
    Moveable(player._uniqueID,player._speed,player._position), 
    _broomstick(NULL), _jersey(NULL), 
    _strength(player._strength), _velocity(player._velocity), _precision(player._precision), _chance(player._chance) 
{}

Player::operator JSON::Dict() const {
    JSON::Dict res = Member::operator JSON::Dict();
    JSON::Dict subres = Moveable::operator JSON::Dict();
    res.stealMerge(subres);
    res.set("strength", _strength);
    res.set("velocity", _velocity);
    res.set("precision", _precision);
    res.set("chance", _chance);
    res.set("name", _name);
    return res;
}

Player& Player::operator=(Player const & player){
    Moveable::operator=(player);
    Member::operator=(player);
    _strength = player._strength;
    _velocity = player._velocity;
    _precision = player._precision;
    _chance = player._chance;
    return *this;
}

float Player::collisionScore() const{
    float res = (2*getStrength()+getVelocity()+getChance());
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
} 

int Player::level () const{
    return static_cast<int>(pow(_strength * _velocity * _precision * _chance, 0.25));
}

// =============================PLAYERQUAFFLE=========================
PlayerQuaffle & PlayerQuaffle::operator= (Player const & other) {
    Player::operator=(other);
    _hasQuaffle = false;
    return *this;
}

// =================================BEATER============================
float Beater::shootBludger () const{ 
    float res = (2*getStrength()+getPrecision()+getChance());
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

/*================================CHASER==============================*/
float Chaser::pass () const {
    float res = getStrength()+2*getPrecision()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

float Chaser::shoot () const {
    float res = 2*getStrength()+getPrecision()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

/*================================KEEPER==============================*/
float Keeper::catchBall () const {
    float res = 2*getPrecision()+getVelocity()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

float Keeper::pass () const {
    float res = getStrength()+2*getPrecision()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

/*================================SEEKER==============================*/
float Seeker::catchGS () const {
    float res = 2*getPrecision()+getVelocity()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}




