#include "randomStateGenerator.hpp"

// RandomState() constructor
// Constructs the random points for the game
// Pre:
// 		None.
RandomState::RandomState() {

	std::srand(std::time(nullptr));

	numberOfCheckersRed_ = std::rand()%16;
	numberOfCheckersBlack_ = std::rand()%16;

	generateRedCheckers();
	generateBlackCheckers();
}

// generateRedCheckers()
// Generates the places the pieces to the board
// Pre:
//		None.
void RandomState::generateRedCheckers() {

	std::srand(std::time(nullptr)*300);
	int randomSpot = 0;

	if(randomSpotRedVec_.size() == numberOfCheckersRed_) {
		randomSpot = std::rand()%16; // First random point in the checkers board

		randomSpotRedVec_.push_back(randomSpot);
		for(int value = 0; value <= numberOfCheckersRed_; ++value) {
			randomSpot = std::rand()%16; // Checker boxes 1-16

			if(checkIfSpotOpen(randomSpotRedVec_, value)) {
				randomSpotRedVec_.push_back(randomSpot);
			}
		}
	}
}

// generateBlackCheckers()
// Generates the places the pieces to the board
// Pre:
//		None.
void RandomState::generateBlackCheckers() {

	std::srand(std::time(nullptr)*100);
	int randomSpot = 0;

	if(randomSpotBlackVec_.size() == numberOfCheckersBlack_) {
		randomSpot = std::rand()%16; // First random point in the checkers board

		randomSpotBlackVec_.push_back(randomSpot);
		for(int value = 0; value <= numberOfCheckersBlack_; ++value) {
			randomSpot = std::rand()%16+16; // Checker boxes 16-32

			if(checkIfSpotOpen(randomSpotBlackVec_, value)) {
				randomSpotBlackVec_.push_back(randomSpot);
			}
		}
	}
}

// checkIfSpotOpen()
// Checks to see if the spot is open to the board
// Pre:
//		None.
bool RandomState::checkIfSpotOpen(std::vector<int> checkerVec, int value) {
	return (std::find(checkerVec.begin(), checkerVec.end(), value) != checkerVec.end());
}