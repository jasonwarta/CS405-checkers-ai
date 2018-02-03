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
		randomSpotRedVec_.push(randomSpot);

		for(auto totalPieces : numberOfCheckersRed_) {
			randomSpot = std::rand()%16; // Checker boxes 1-16

			if(checkIfSpotOpen(numberOfCheckersRed_)) {
				randomSpotRedVec_.push(randomSpot);
			}
			else {
				generateRedCheckers();
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

	if(randomSpotBlack_.size() == numberOfCheckersBlack_) {
		randomSpot = std::rand()%16; // First random point in the checkers board

		randomSpotBlack_.push(randomSpot);
		for(auto totalPieces : numberOfCheckersBlack_) {
			randomSpot = std::rand()%16+16; // Checker boxes 16-32

			if(checkIfSpotOpen(numberOfCheckersBlack_)) {
				randomSpotBlackVec_.push(randomSpot);
			}
		}
	}
}

// checkIfSpotOpen()
// Checks to see if the spot is open to the board
// Pre:
//		None.
bool RandomState::checkIfSpotOpen(std::vector<int> checkerVec) {
	return std::find(checkerVec.begin(),checkerVec.end(), checkerVec.size());
}