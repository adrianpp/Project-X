#ifndef TILE_H__
#define TILE_H__

#include "Minion.hpp"
#include <string>

class tile 
{
public:
	tile() {
		symbol = '.';
		defenseFactor = 1.1;
		description = "Clear";
		hasMinionOnTop = false;
		minionOnTop = NULL;
		withinRange = false;
		minionOnTop = NULL;
		controller = 0;
		production = 0;
	}


	int production;				//Amount of money it produces
	int controller;				//Player number 1/2 or neutral 0.
	char symbol;
	bool hasMinionOnTop;
	Minion* minionOnTop;
	bool withinRange;
	std::string description;
	double defenseFactor;		//Higher is better (ie mountains are 1.4)
	int clearTile() {
		symbol = '.';
		defenseFactor = 1.1;
		description = "Clear";
		hasMinionOnTop = false;
		minionOnTop = NULL;
		withinRange = false;
		minionOnTop = NULL;
		controller = 0;
		production = 0;
		return 0;
	}


};

#endif /* TILE_H__ */

