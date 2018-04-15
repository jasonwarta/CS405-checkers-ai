// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, int depth) {
	if(usingIterativeDeepening)
	{
 		Clock theClock = std::chrono::system_clock::now();
 		MinimaxWithAlphaBeta bestTestingEver(theBoard,depth,redTeam_,net_, &theClock);
		std::cout << "TESTING: " << bestTestingEver.getBestVector().size() << std::endl;
		return bestTestingEver.getBestBoard(os);
	}
	else
	{
		// MinimaxWithAlphaBeta searchTree(theBoard,depth,redTeam_,net_,usingPieceCount_);
		if (!usingPieceCount_)
			return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,net_).getBestBoard(os);
		else
			return MinimaxWithAlphaBeta(theBoard, depth, redTeam_).getBestBoard(os);
	}
}
