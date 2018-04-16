// Player.h

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <math.h>
#include <memory>

#include "MinimaxWithAlphaBeta.h"
#include "defs.h"
#include "consts.h"

class Player
{
public:
	Player( bool redTeam ) : redTeam_(redTeam) {};
	Player( bool redTeam, NeuralNet *net, bool usingPieceCount = false) : 
		redTeam_(redTeam), 
		net_(net),
		usingPieceCount_(usingPieceCount) {};

	std::string getMove(std::string &theBoard, std::ostream *os, uint depth = 4);
private:
	bool redTeam_;
	NeuralNet *net_;
	bool usingPieceCount_;
};

#endif