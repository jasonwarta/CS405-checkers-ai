// Player.h

#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <string>
#include <math.h>
#include <memory>

#include "MinimaxWithAlphaBeta.h"
#include "NN91_Basic.h"

typedef std::chrono::time_point<std::chrono::system_clock> Clock;

class Player
{
public:
	Player( bool redTeam, std::shared_ptr<Clock> clock ) : redTeam_(redTeam), clock_(clock) {};
	Player( bool redTeam, std::shared_ptr<Clock> clock, NN91_Basic *net) : 
		redTeam_(redTeam), 
		clock_(clock),
		net_(net) {};

	std::string getMove(std::string &theBoard, std::ostream *os, int depth = 4);
private:
	bool redTeam_;
	std::shared_ptr<Clock> clock_;
	NN91_Basic *net_;
};

#endif