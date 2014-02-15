#ifndef Team_hpp
#define Team_hpp

#include "Gear.hpp"
#include "Sponsor.hpp"
#include "Installation.hpp"
#include <string>
#include <list>

/*
Think about :
- Determinate consts values
- Already got a sponsor at start ? Coach ?
- Nbr starting players ? (6?)
- System to create random Players ? (constructor in class Player to initialize stats depending on position)
- update installations (Fanshop, MedicalCenter, AlimentationService, Stand)
- create Stadium class which contents the installations of a team 
*/

//Consts
std::string UNNAMED_TEAM = "unnamed_";
int MAX_COACHES = 5;
int MAX_PLAYERS = 15;
int STARTING_FUNDS = 0;
int STARTING_FAME = 0;

class Team{
private:
    std::string _name;
	std::list<Player> _players;
	std::list<Coach> _coaches;
	std::list<Gear> _gear;
	Stadium _stadium;
	Sponsor _sponsor;
	int _fame;
	int _funds;
	int _nbrWins;
	float _nbrPlayed;
	static int _nbrUnnamedTeams;	//Used in default constructor

	void createStartingPlayers();
public:
	//Constructors
	Team::Team(): _name(UNNAMED_TEAM+_nbrUnnamedTeams), _fame(STARTING_FAME), _funds(STARTING_FUNDS), _nbrWin(0), _nbrPlayed(0){
		++_nbrUnnamedTeams;
		createStartingPlayers();
	}
	Team::Team(name): _name(name), _fame(STARTING_FAME), _funds(STARTING_FUNDS), _nbrWins(0), _nbrPlayed(0){createStartingPlayers();}

	//Getters
	std::string getName(){return _name;}
	int getFunds(){return _funds;}
	int getFame(){return _fame;}
	int getWins(){return _nbrWins;}
	int getPlayed(){return _nbrPlayed;}
	float getRatio (){return nbrWins/nbrPlayed;}
	Stadium getStadium(){return _stadium;}
	Sponsor getSponsor(){return _sponsor;}
	std::list<Player> getPlayers(){return _players;}
	std::list<Coach> getCoaches(){return _coaches;}
	std::list<Gear> getGear(){return _gear;}

	//Setters
	void setFunds(int newValue){_funds = newValue;}
	void setFame(int newValue){_fame = newValue;}
	void played++(){++_nbrPlayed;}
	void wins++(){++_nbrWins;}


	void newSponsor(Sponsor);
	bool firePlayer(Player);
	bool fireCoach(Coach);
	void addPlayer(Player player);
	void addCoach(Coach coach);
};

//Static data member init
int Team::_nbrUnnamedTeams = 0;



#endif
