// Player.h

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <math.h>
#include <memory>

#include "MinimaxWithAlphaBeta.h"
#include "defs.h"

class Player
{
public:
	Player( bool redTeam, std::shared_ptr<Clock> clock ) : redTeam_(redTeam), clock_(clock) {};
	Player( bool redTeam, std::shared_ptr<Clock> clock, NeuralNet *net) : 
		redTeam_(redTeam), 
		clock_(clock),
		net_(net) {};

	std::string getMove(std::string &theBoard, std::ostream *os, int depth = 4);
private:
	bool redTeam_;
	std::shared_ptr<Clock> clock_;
	NeuralNet *net_;
};

#endif