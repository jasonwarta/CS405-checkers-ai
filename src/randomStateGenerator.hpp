#ifndef RANDOM_STATE
#define RANDOM_STATE

#include <string>
using std::string;
#include <cstdint>
#include <ctime>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#include "checkers.h"

// Class RandomState
// Starts the checkers board with a new state
// Pre:
//		None. 
class RandomState {
public:
	RandomState();

	void generateRedCheckers();
	void generateBlackCheckers();
	bool checkIfSpotOpen(std::std::vector<int>);

private:
	int16_t numberOfCheckersRed_;
	int16_t numberOfCheckersBlack_;

	std::vector<int> randomSpotRedVec_;
	std::vector<int> randomSpotBlackVec_;
};

#endif // RANDOM_STATE