#ifndef Member_hpp
#define Member_hpp  

class Member 
{
public:
	string getName ();
	int getSalary ();
	int getPrice ();
	Team getOwner ();
	void setOwner (Team aTeam);
private:
	string name;
	int salary;
	int price;
	Team owner;
};

#endif
