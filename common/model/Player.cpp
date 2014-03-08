#include "Player.hpp"
#include <model/MemoryAccess.hpp>

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
    
Player::Player(JSON::Dict const & json) : Member(json), Moveable(json), 
				_maxLife(100), _maxMana(100), _lifeBar(100), 
                _manaBar(100), _broomstick(), 
                _jersey(), _strength(5), _velocity(5), _precision(5), _chance(5) {
		if (ISINT(json.get("memberID"))) _memberID = INT(json.get("memberID")).value();
		if (ISSTR(json.get("name"))) _name = STR(json.get("name")).value();
        if (ISINT(json.get("maxLife"))) _maxLife = INT(json.get("maxLife")).value();
        if (ISINT(json.get("maxMana"))) _maxMana = INT(json.get("maxMana")).value();
        if (ISINT(json.get("lifeBar"))) _lifeBar = INT(json.get("lifeBar")).value();
        if (ISINT(json.get("manaBar"))) _manaBar = INT(json.get("manaBar")).value();
        if (ISDICT(json.get("broomstick"))) 
            _broomstick = new Broomstick(DICT(json.get("broomstick")));
        else
            _broomstick = new Broomstick();
        if (ISDICT(json.get("jersey"))) 
            _jersey = new Jersey(DICT(json.get("jersey")));
        else
            _jersey = new Jersey();
        if (ISINT(json.get("strength"))) _strength = INT(json.get("strength")).value();
        if (ISINT(json.get("velocity"))) _velocity = INT(json.get("velocity")).value();
        if (ISINT(json.get("precision"))) _precision = INT(json.get("precision")).value();
        if (ISINT(json.get("chance"))) _chance = INT(json.get("chance")).value();
}

Player::Player(const Player & player) : Member(player._memberID,player._name,player._salary,player._price,player._owner), 
                Moveable(player._uniqueID,player._speed,player._position), 
                _maxLife(player._maxLife), _maxMana(player._maxMana),_lifeBar(player._lifeBar), 
                _manaBar(player._manaBar), _broomstick(), 
                _jersey(), _strength(player._strength), _velocity(player._velocity), 
                _precision(player._precision), _chance(player._chance) {
        _broomstick = new Broomstick(*(player._broomstick));
        _jersey = new Jersey(*(player._jersey));
}//modif

Player::operator JSON::Dict() const {
    JSON::Dict res = Member::operator JSON::Dict();
    JSON::Dict subres = Moveable::operator JSON::Dict();
    res.stealMerge(subres);
    res.set("maxLife", _maxLife);
    res.set("maxMana", _maxMana);
    res.set("lifeBar", _lifeBar);
    res.set("manaBar", _manaBar);
    res.set("broomstick", JSON::Dict(*_broomstick));
    res.set("jersey", JSON::Dict(*_jersey));
    res.set("strength", _strength);
    res.set("velocity", _velocity);
    res.set("precision", _precision);
    res.set("chance", _precision);
    res.set("name", _name);
    return res;
}

//void Player::save(){}


// Player* Player::load(int id, std::string username){
//     MemoryAccess::load()
//     JSON::Value *loaded = JSON::load(getPlayersFile(username).c_str());
//     JSON::List & players = *((JSON::List*)loaded);
//     for(size_t i =0; i<players.len();++i){
//         if(INT(DICT(players[i]).get(net::MSG::PLAYER_ID)) == id){
//             delete loaded;
//             return (new Player(DICT(players[i])));
//         }
//     }
//     delete loaded;
//     return NULL;
// }


void Player::equipBroomstick (Broomstick aBroom){
    *_broomstick = aBroom;
}

void Player::equipJersey (Jersey aJersey){
    *_jersey = aJersey;
}

void Player::recoverLife (){
    _lifeBar = _maxLife;
}

void Player::loseLife (int damage){
    _lifeBar -= damage;
    if (_lifeBar < 0)
        _lifeBar = 0 ;
}

void Player::recoverMana (){
    _manaBar = _maxMana;
}

void Player::loseMana (int spelled){
    _manaBar -= spelled;
    if (_manaBar < 0)
        _manaBar = 0;
}

float Player::collisionScore() const{
    float res = (2*getStrength()+getVelocity()+getChance());
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
} 

// BEATER ----------------------------------------------------------------------
void Beater::equipBat(Bat aBat){
    *_bat = aBat;
}

float Beater::shootBludger () const{ 
    float res = (2*getStrength()+getPrecision()+getChance());
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}

// CHASER ----------------------------------------------------------------------
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

// KEEPER ----------------------------------------------------------------------
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

// SEEKER ----------------------------------------------------------------------
float Seeker::catchGS () const {
    float res = 2*getPrecision()+getVelocity()+getChance();
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
}




