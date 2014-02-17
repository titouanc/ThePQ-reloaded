#ifndef ParticipatingTeam_hpp
#define ParticipatingTeam_hpp

class ParticipatingTeam 
{
public:
	ParticipatingTeam();
	void addPoints (int toAdd);
	int getPoints ();
	void changeGoalAverage (int toChange);
	int getGoalAverage ();
    std::string getName ();
	int getLevel ();
	~ParticipatingTeam();
private:
	int points;
	int goalAverage;
    std::string name;
	int level;
};

#endif
