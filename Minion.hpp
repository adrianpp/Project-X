#ifndef MINION_HPP__
#define MINION_HPP__

#include <string>


//Forward declaration for MasterBoard.
class MasterBoard;
enum minionStatus { hasntmovedorfired, hasmovedhasntfired, hasfired, gaveupmovehasntfired};
enum RangeType {directFire, rangedFire};
enum specialtyGroupType {normal, infantry};


class Minion
{	
	public:
	
	//Constructors
	Minion(int inputSeniority, int inputX, int inputY, char inputType, int inputTeam, MasterBoard* Environment, int inputHealth, int inputVeterancy);
	Minion();
	~Minion();
	
	
	//Member variables. Should be protected with functions dealing with them!
	
	int seniority;		//Used to indicated where the minion resides within the minion array. Used to construct and destruct the minion.
	int locationX;
	int locationY;
	char type;			//Type of minion i.e. infantry, artillery, etc.
	int movementRange;
	int attackRange;
	int visionRange;
	bool isCapturing = false;
	std::string description;	//For user interface only.
	int team;
	double health;
	std::string Image = { '.','.','.',
						'.','.','.',
						'.','.','.' };
	int veterancy = 0;

	double calculateVetBonus();

	MasterBoard* minionEnvironment;
	minionStatus status;
	RangeType rangeType;
	specialtyGroupType specialtyGroup;

};



#endif /* MINION_HPP__ */

