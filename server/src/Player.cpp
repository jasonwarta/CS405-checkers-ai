// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, int depth) {
	return MinimaxWithAlphaBeta(theBoard,depth,redTeam_,net_,usingPieceCount_).getBestBoard(os);
}
