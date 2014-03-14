#include "Player.hpp"
#include <model/MemoryAccess.hpp>
#include <cmath>

std::ostream& operator<< (std::ostream& out, const Player& player)
    {
        out << "\033[35m#" << player._memberID
            << " \033[1m" << player._name 
            << " : \033[0mLife(\033[32m" << player._lifeBar << "\033[0m/"<< player._maxLife 
            << ") Mana(\033[32m" << player._manaBar << "\033[0m/" << player._maxMana 
            << ") Strength(\033[32m" << player._strength 
            << "\033[0m) Velocity(\033[32m" << player._velocity 
            << "\033[0m) Precision(\033[32m" << player._precision 
            << "\033[0m) Chance (\033[32m" << player._chance << ")\033[0m";
        return out;
    }
    
Player::Player(JSON::Dict const & json) : 
    Member(json), Moveable(json), 
	_maxLife(100), _maxMana(100), _lifeBar(100), _manaBar(100), 
     _broomstick(NULL), _jersey(NULL), 
     _strength(5), _velocity(5), _precision(5), _chance(5) {

	//if (ISINT(json.get("memberID")))     _memberID = INT(json.get("memberID"));
	if (ISSTR(json.get("name")))         _name = STR(json.get("name")).value();
    if (ISINT(json.get("maxLife")))      _maxLife = INT(json.get("maxLife"));
    if (ISINT(json.get("maxMana")))      _maxMana = INT(json.get("maxMana"));
    if (ISINT(json.get("lifeBar")))      _lifeBar = INT(json.get("lifeBar"));
    if (ISINT(json.get("manaBar")))      _manaBar = INT(json.get("manaBar"));
    if (ISINT(json.get("strength")))     _strength = INT(json.get("strength"));
    if (ISINT(json.get("velocity")))     _velocity = INT(json.get("velocity"));
    if (ISINT(json.get("precision")))    _precision = INT(json.get("precision"));
    if (ISINT(json.get("chance")))       _chance = INT(json.get("chance"));
}

Player::Player(const Player & player) : 
    Member(player._memberID,player._name,player._salary,player._price,player._owner), 
    Moveable(player._uniqueID,player._speed,player._position), 
    _maxLife(player._maxLife), _maxMana(player._maxMana),_lifeBar(player._lifeBar), _manaBar(player._manaBar), 
    _broomstick(NULL), _jersey(NULL), 
    _strength(player._strength), _velocity(player._velocity), _precision(player._precision), _chance(player._chance) 
{}

Player::operator JSON::Dict() const {
    JSON::Dict res = Member::operator JSON::Dict();
    JSON::Dict subres = Moveable::operator JSON::Dict();
    res.stealMerge(subres);
    res.set("maxLife", _maxLife);
    res.set("maxMana", _maxMana);
    res.set("lifeBar", _lifeBar);
    res.set("manaBar", _manaBar);
    res.set("strength", _strength);
    res.set("velocity", _velocity);
    res.set("precision", _precision);
    res.set("chance", _precision);
    res.set("name", _name);
    return res;
}

Player& Player::operator=(Player const & player){
    Moveable::operator=(player);
    Member::operator=(player);
    _maxLife = player._maxLife;
    _maxMana = player._maxMana;
    _lifeBar = player._lifeBar;
    _manaBar = player._manaBar;
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




