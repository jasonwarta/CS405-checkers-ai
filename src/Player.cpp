// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, int depth, std::ostream *os) {
	if (kingWeight_ == kingWeight_)
		return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,kingWeight_,clock_).getBestBoard(os);
	else
		return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,clock_).getBestBoard(os);
}
