// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, int depth) {
	// MinimaxWithAlphaBeta searchTree(theBoard,depth,redTeam_,net_,usingPieceCount_);
	if (!usingPieceCount_)
		return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,net_).getBestBoard(os);
	else
		return MinimaxWithAlphaBeta(theBoard, depth, redTeam_).getBestBoard(os);
}
