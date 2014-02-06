#ifndef Displacement_hpp
#define Displacement_hpp

class Displacement : Action {
public:
	Displacement();
	Displacement(int length);
	~Displacement();
	bool isLegal();
	
private:
	unsigned int _length;
};

#endif