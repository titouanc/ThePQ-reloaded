#include "Player.hpp"
#include <model/MemoryAccess.hpp>

std::ostream& operator<< (std::ostream& out, const Player& player)//modif
    {
        out << "---------------------------------------------------" << std::endl;
        out << "Player:         " << player._name << std::endl;
        out << "Life:           " << player._lifeBar << "/" << player._maxLife << std::endl;
        out << "Mana:           " << player._manaBar << "/" << player._maxMana << std::endl;
        out << "Strength:       " << player._strength << std::endl;
        out << "Constitution:   " << player._constitution << std::endl;
        out << "Magic:          " << player._magic << std::endl;
        out << "Spirit:         " << player._spirit << std::endl;
        out << "Velocity:       " << player._velocity << std::endl;
        out << "Precision:      " << player._precision << std::endl;
        out << "Chance:         " << player._chance << std::endl;
        out << "ID:             " << player._memberID << std::endl;//modif
        //TODO : display mum and dad (aka super classes)
        return out;
    }
    
Player::Player(JSON::Dict const & json) : Member(json), Moveable(json), 
				_maxLife(100), _maxMana(100), _lifeBar(100), 
                _manaBar(100), _broomstick(), 
                _jersey(), _strength(5), _constitution(5), 
                _magic(5), _spirit(5), _velocity(5), _precision(5), _chance(5) {
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
        if (ISINT(json.get("constitution"))) _constitution = INT(json.get("constitution")).value();
        if (ISINT(json.get("magic"))) _magic = INT(json.get("magic")).value();
        if (ISINT(json.get("spirit"))) _spirit = INT(json.get("spirit")).value();
        if (ISINT(json.get("velocity"))) _velocity = INT(json.get("velocity")).value();
        if (ISINT(json.get("precision"))) _precision = INT(json.get("precision")).value();
        if (ISINT(json.get("chance"))) _chance = INT(json.get("chance")).value();
}

Player::Player(const Player & player) : Member(player._memberID,player._name,player._salary,player._price,player._owner), 
                Moveable(player._uniqueID,player._speed,player._position), 
                _maxLife(player._maxLife), _maxMana(player._maxMana),_lifeBar(player._lifeBar), 
                _manaBar(player._manaBar), _broomstick(), 
                _jersey(), _strength(player._strength), _constitution(player._constitution), 
                _magic(player._magic), _spirit(player._spirit), _velocity(player._velocity), 
                _precision(player._precision), _chance(player._chance) {
        _broomstick = new Broomstick(*(player._broomstick));
        _jersey = new Jersey(*(player._jersey));
}//modif

Player::operator JSON::Dict(){
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
    res.set("constitution", _constitution);
    res.set("magic", _magic);
    res.set("spirit", _spirit);
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
    //improveVelocity(-1 * _broomstick->getVelocityBonus());
    //improveVelocity(aBroom.getVelocityBonus());//modif
    *_broomstick = aBroom;
}

void Player::equipJersey (Jersey aJersey){
    // improveStrength(-1 * _jersey->getStrengthBonus());
    // improveConstitution(-1 * _jersey->getConstitutionBonus());
    // improveMagic(-1 * _jersey->getMagicBonus());
    // improveSpirit(-1 * _jersey->getSpiritBonus());
    // improveStrength(aJersey.getStrengthBonus());
    // improveConstitution(aJersey.getConstitutionBonus());
    // improveMagic(aJersey.getMagicBonus());
    // improveSpirit(aJersey.getSpiritBonus());
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

float Player::collisionScore(){
    float res = (getStrength()+getConstitution()+getVelocity()+getChance());
    res = res * (100000000 +(rand()%10000000))/100000000;
    return res;
} 

// BEATER ----------------------------------------------------------------------
void Beater::equipBat(Bat aBat){
    *_bat = aBat;
}

void Beater::unequipBat(){
}

int Beater::collision (){
}
int Beater::anticollision (){
}

int Beater::shootBludger (){ 
}

// CHASER ----------------------------------------------------------------------
int Chaser::speed (){
}

int Chaser::collisionner (){
}

int Chaser::anticollision (){
}

int Chaser::pass (){
}

int Chaser::shoot (){
}

// KEEPER ----------------------------------------------------------------------
int Keeper::catchBall (){
}

int Keeper::pass (){
}

// KEEPER ----------------------------------------------------------------------
int Seeker::catchGS (){
}




