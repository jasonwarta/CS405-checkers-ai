// Player.cpp

#include "Player.h"

std::string Player::getMove(std::string &theBoard, std::ostream *os, uint depth) {
	return MinimaxWithAlphaBeta(theBoard, redTeam_, depth, depth, net_).getBestBoard(os);
}
