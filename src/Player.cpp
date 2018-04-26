// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, uint minDepth, uint maxDepth)
{
	return MinimaxWithAlphaBeta(theBoard, redTeam_, minDepth, maxDepth, net_).getBestBoard(os);
}
