// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, int depth) {
	// return MinMaxTree(theBoard,depth,redTeam_,net_).getBestBoard();
	return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,clock_,net_,usingPieceCount_).getBestBoard(os);
}